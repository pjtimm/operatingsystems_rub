#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "plist.h"  

// Callback für walkList
bool check_zombie(pid_t pid, const char *cmdLine) {
    int status;
    pid_t result = waitpid(pid, &status, WNOHANG);
    if (result == pid) {
        printf("Exitstatus [%s] = %d\n", cmdLine, WEXITSTATUS(status));
        char buffer[1024];
        removeElement(pid, buffer, sizeof(buffer));  // redundanter Clean-up
        return true;  // Element aus Liste entfernen
    }
    return false;  // Prozess lebt noch
}

bool print_job(pid_t pid, const char *cmdLine) {
    printf("%d %s\n", pid, cmdLine);
    return false;  // NICHT aus Liste entfernen
}


int main() {
    while (true) {
        // 🧹 Beendete Hintergrundprozesse aufsammeln
        walkList(check_zombie);

        // 📍 Pfadlänge bestimmen
        long max_path = pathconf(".", _PC_PATH_MAX);
        if (max_path == -1) {
            perror("Konnte maximale Pfadlänge nicht bestimmen");
            return 1;
        }

        char *path = malloc(max_path);
        if (!path) {
            perror("malloc path");
            return 1;
        }

        if (getcwd(path, max_path) == NULL) {
            perror("getcwd");
            free(path);
            return 1;
        }

        // Prompt anzeigen
        printf("%s: ", path);
        fflush(stdout);

        char *command = malloc(max_path);
        if (command==NULL) {
            perror("malloc command");
            free(path);
            return 1;
        }

        if (fgets(command, max_path, stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");
                free(command);
                free(path);
                break;  // EOF → Shell beenden
            } else {
                perror("Fehler beim Lesen");
                free(command);
                free(path);
                continue;
            }
        }

        command[strcspn(command, "\n")] = '\0';  // Newline entfernen

        // Tokenisierung
        char **args = NULL;
        int counter = 0;
        char *file = strtok(command, " \t");
        if (!file) {
            fprintf(stderr, "Kein Befehl gefunden\n");
            free(command);
            free(path);
            continue;
        }

        args = realloc(args, (counter + 2) * sizeof(char *));
        args[counter++] = file;

        char *token = strtok(NULL, " \t");
        while (token != NULL) {
            args = realloc(args, (counter + 2) * sizeof(char *));
            if (!args) {
                perror("realloc");
                exit(1);
            }
            args[counter++] = token;
            token = strtok(NULL, " \t");
        }
        args[counter] = NULL;

        // Hintergrundprozess erkennen
        bool background = false;
        if (counter > 0 && strcmp(args[counter - 1], "&") == 0) {
            background = true;
            args[counter - 1] = NULL;
        }
        
        if (strcmp(file, "jobs") == 0) {
            walkList(print_job);

            // cleanup und weiter zum nächsten Prompt
            free(args);
            free(command);
            free(path);
            continue;
        }

        if (strcmp(file, "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Pfad fehlt für cd\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("chdir fehlgeschlagen");
            }
            }
            // cleanup und weiter zum nächsten Prompt
            free(args);
            free(command);
            free(path);
            continue;
        }



        // Kindprozess erzeugen
        pid_t pid = fork();
        if (pid == 0) {
            execvp(file, args);
            perror("execvp fehlgeschlagen");
            exit(1);
        } else if (pid > 0) {
            if (background) {
                insertElement(pid, strdup(command));  // 🗃 In Liste eintragen, wird im Hintergrund ausgeführt
            } else {
                int status;
                if (waitpid(pid, &status, 0) == -1) {
                    perror("waitpid");
                } else {
                    printf("Exitstatus [");
                    for (int i = 0; args[i] != NULL; ++i) {
                        printf("%s%s", args[i], args[i + 1] ? " " : "");
                    }
                    printf("] = %d\n", WEXITSTATUS(status));
                }
            }
        } else {
            perror("Prozess konnte nicht erzeugt werden");
        }

        free(args);
        free(command);
        free(path);
    }

    return 0;
}
