#include <stdio.h>
#include <string.h>

#define MAX_WORDS 100

int main() {
    char delimiters[] = "!.-:;? \n\t";
    char input[100];
    char* words[MAX_WORDS];
    int wordCount = 0;
    
    printf("Enter multiple lines of text (Press Ctrl+D to end):\n");
    
    while (fgets(input, sizeof(input), stdin)) {
        char* word = strtok(input, delimiters);
        
        while (word != NULL) {
            words[wordCount++] = word;
            word = strtok(NULL, delimiters);
        }
    }
    
    printf("Addresses of the words:\n");
    for (int i = 0; i < wordCount; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}