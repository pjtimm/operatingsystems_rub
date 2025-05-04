#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reverse_comp(const void *a, const void *b);

int main(void){
    #define INITIAL_CAPACTIY 100
   
    char **words = malloc(INITIAL_CAPACTIY * sizeof(char *)); //pointer to point at the individual words which are of type (char *)
    if(!words){
        perror("initial malloc failed");
        exit(EXIT_FAILURE);
    }
    int capacity = INITIAL_CAPACTIY;
    int word_count = 0;

    char BUFFER[102]; // 100 +1 for newline and +1 for detecting overflow
    int line_num = 1; //keep seperate account of lines and words as some lines may be skipped an not count towards the word count

    //reading the file
    while(fgets(BUFFER,102,stdin)){
        if(strchr(BUFFER,'\n') == NULL && !feof(stdin)){ //Check if line exceeds character limit
            fprintf(stderr, "Line %d exceeded 100 charactes", line_num);
        }

        else{
            //provide more memory if needed
            if (word_count >= capacity) {
                capacity *= 2;
                words = realloc(words, capacity * sizeof(char *));
                if (!words) {
                    perror("realloc failed");
                    exit(EXIT_FAILURE);
                }
            }

            //storing the word
            int string_length = strlen(BUFFER) + 1; //+1 for null terminator
            BUFFER[strcspn(BUFFER, "\n")] = '\0'; //search for newline and exchange it with null terminator
            words[word_count] = malloc(string_length); 
            if(!words[word_count]){
                perror("malloc for individual word failed");
                exit(EXIT_FAILURE);
            }
            strcpy(words[word_count],BUFFER); //copy word into the words array
            word_count++;
        }
        line_num++;
    }


    //sorting the words
    qsort(words,word_count,sizeof(char *),reverse_comp);

    //output
    for(int i = 0; i < word_count; i++){
        printf("%s\n", words[i]);
        free(words[i]);
    }

    free(words);
    return 0;
}

int reverse_comp(const void *a, const void *b){ //arguments as required by qsort
    const char *wordA = *(const char **)a; //cast the generic pointer to a char ** as words is defined
    const char *wordB = *(const char **)b;
    return strcmp(wordB, wordA);  //positive if wordB > wordA
}

