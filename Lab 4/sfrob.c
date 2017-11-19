//
//  main.c
//  tester
//
//  Created by Luca Matsumoto on 10/28/17.
//  Copyright © 2017 Luca Matsumoto. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>


int frobcmp(char const* a, char const* b){
    for(;;a++, b++){
        if(*a == ' ' && *b == ' '){
            return 0;
        }
        else if(*b == ' ' || (*a^42) > ((*b^42))){
            return 1;
        }
        else if(*a == ' ' || (*a^42) < ((*b^42))){
            return -1;
        }
    }
}//similar to C strcmp implementation, except with frobnicated char

int comparator(const void* ptr1, const void* ptr2) {
    char const* str1 = *(const char**) ptr1;
    char const* str2 = *(const char**) ptr2;
    return frobcmp(str1, str2);
}//compare function for  qsort

void checkError(FILE* fp) {
    if(ferror(fp)){
        fprintf(stderr, "Could not read file");
        exit(1);
    }
}

void checkMemError(void* ptr) {
    if(ptr == NULL) {
        fprintf(stderr, "Memory Allocation Error");
        exit(1);
    }
}

void freeWords(int wordCount, char** words) {
    int i;
    for(i = 0; i < wordCount; i++) {
        free(words[i]);
    }
    free(words);
}


int main(void) {
    char* str;
    char** words;
    str = (char*)malloc(sizeof(char));//array of chars (string)
    words = (char**)malloc(sizeof(char*)); //array of strings (pointer to char*)
    int wordCount = 0;
    char current = getchar();
    checkError(stdin);
    int letterCount = 0;
    while (current != EOF) {
        while (current != ' ' && current != EOF) {
            str[letterCount] = current;
            char* temp = realloc(str, (letterCount+2)*sizeof(char));
            if(temp == NULL){
                free(str);
                checkMemError(temp);
            }
            else{
                str = temp;
                letterCount++;
                current = getchar();
                checkError(stdin);
            }
        }
        if (letterCount > 0) {
            str[letterCount] = ' ';
        }
        while (current == ' ') {
            current = getchar();
            checkError(stdin);
        }
        letterCount = 0;
        words[wordCount] = str;
        char** wordTemp = realloc(words, (wordCount+2)*sizeof(char*));
        if (wordTemp == NULL){
            free(words);
            checkMemError(wordTemp);
        }
        else {
            words = wordTemp;
            wordCount++;
            str = NULL;
            str = (char*)malloc(sizeof(char));
        }
    }
    
    qsort(words, wordCount, sizeof(char*), comparator);
    int i, j;
    for(i = 0; i < wordCount; i++){
        for(j = 0;; j++){
            if(words[i][j] == ' '){
                putchar(words[i][j]);
                break;
            }
            if(putchar(words[i][j]) == EOF){
                fprintf(stderr, "Error writing character");
                exit(1);
            }
        }
    }
    freeWords(wordCount, words);
}


