#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// Vergleichsfunktion für qsort – sortiert absteigend alphabetisch (umgekehrte Reihenfolge)
int compare(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcmp(s2, s1);  // Rückwärts: s2 vor s1 = absteigend
}

int rsort();
// Hauptfunktion: ruft rsort auf und gibt dessen Rückgabewert zurück
int main(void) {
    return rsort();
}

// Funktion zum Einlesen, Sortieren und Ausgeben der Wörter
int rsort(void) {
    char newWord[101];             // Puffer für ein eingelesenes Wort (max. 100 Zeichen + \0)
    int anzahl = 0;                // Anzahl der gültigen Wörter
    char **liste = NULL;           // Dynamisch wachsende Liste von String-Zeigern

    // Wörter aus stdin einlesen
    while (fgets(newWord, sizeof(newWord), stdin) != NULL) {
        size_t len = strlen(newWord);

        // Wörter dürfen nicht länger als 100 Zeichen lang sein
       if (len > 0 && newWord[len - 1] != '\n') {
            fprintf(stderr, "Das Wort '%s...' ist zu lang und wird ignoriert.\n", newWord);

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        newWord[strcspn(newWord, "\n")] = '\0';

        if (strlen(newWord) == 0) continue;      // Leere Zeile ignorieren

        // Speicherplatz für einen weiteren Eintrag reservieren
        char **temp = realloc(liste, (anzahl + 1) * sizeof(char *));
        if (temp == NULL) {
            perror("realloc fehlgeschlagen");
            for (int i = 0; i < anzahl; i++) free(liste[i]);
            free(liste);
            return EXIT_FAILURE;
        }
        liste = temp;

        // Wort duplizieren und einfügen
        liste[anzahl] = strdup(newWord);
        if (liste[anzahl] == NULL) {
            perror("strdup fehlgeschlagen");
            for (int i = 0; i < anzahl; i++) free(liste[i]);
            free(liste);
            return EXIT_FAILURE;
        }

        anzahl++;
    }

    // Fehler beim Einlesen prüfen
    if (ferror(stdin)) {
        perror("Fehler beim Lesen");
        for (int i = 0; i < anzahl; i++) free(liste[i]);
        free(liste);
        return EXIT_FAILURE;
    }

    // Wörter sortieren
    qsort(liste, anzahl, sizeof(char *), compare);

    // Ausgabe & Speicherfreigabe
    for (int i = 0; i < anzahl; i++) {
        if (printf("%s\n", liste[i]) < 0) {
            perror("Fehler beim Schreiben");
            for (int j = i; j < anzahl; j++) free(liste[j]);
            free(liste);
            return EXIT_FAILURE;
        }
        free(liste[i]);
    }

    free(liste);
    return EXIT_SUCCESS;
}
