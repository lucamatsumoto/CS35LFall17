#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include <ctype.h>

int f_selected = 0;
int compare = 0;
int decode(char a){
    int val;
    if(f_selected == 1){
        val = toupper((unsigned char)a);
    }
    else {
        val = (int)a;
    }
    return val;
}


int frobcmp(char const* a, char const* b){
    for(;;a++, b++){
        if(*a == ' ' && *b == ' '){
            return 0;
        }
        else if(*b == ' ' || (decode(*a^42) > decode(*b^42))){
            return 1;
        }
        else if(*a == ' ' || (decode(*a^42) < decode(*b^42))){
            return -1;
        }
    }
}//similar to C strcmp implementation, except with frobnicated char

int comparator(const void* ptr1, const void* ptr2) {
    char const* str1 = *(const char**) ptr1;
    char const* str2 = *(const char**) ptr2;
    compare++;
	return frobcmp(str1, str2);
}//compare function for  qsort

void checkError(ssize_t state){
    if(state < 0){
        fprintf(stderr, "Could not read file: Error!");
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


int main(int argc, char* argv[]) {
    if(argc == 2 || argc == 3){
        if((strncmp(argv[1], "-f", 2)) == 0){
            f_selected = 1;
        }
        else{
            fprintf(stderr, "invalid option");
	    exit(1);
        }
    }
    struct stat fileStat;
    if(fstat(0, &fileStat) < 0){
        fprintf(stderr, "Error!");
        exit(1);
    }
    char* str;
    char** words;
    char* files;
    int wordCount = 0;
    if(S_ISREG(fileStat.st_mode)){
        files = (char*)malloc(fileStat.st_size*sizeof(char));
        ssize_t state = read(0, files, fileStat.st_size);
        int spaceCount; //tracking every new word so that we can create words array
        int increment = 1;
        if(state > 0){ //file read properly
            int i;
            for(i = 0; i < fileStat.st_size; i+=increment){
                increment = 1;
                if(files[i] == ' '){
                    while(files[i] != ' '){
                        i++;
                        increment++;
                    }
                }
                if(i == fileStat.st_size - 1){
                    files[i] = ' '; //end of file must include space
                }
                spaceCount++;
            }
        }
        words = (char**)malloc(spaceCount*sizeof(char*));
        int check = 0; //bool to check word
        int j;
        for(j = 0; j < fileStat.st_size; j++){
            if(files[j] != ' ' && check == 0){
                words[wordCount] = &files[j];
                wordCount++;
                check = 1;
            }
            else if(check == 1 && files[j] == ' '){
                check = 0;
            }
        }
    }
    else {
        words = (char**)malloc(sizeof(char*)); //array of strings (pointer to char*)
    }
    
    str = (char*)malloc(sizeof(char));//array of chars (string)
    char current[1];
    ssize_t state = read(0, current, 1);//read 1 byte at a time
    checkError(state);
    int letterCount = 0;
    while (state > 0) {
        while (current[0] != ' ' && state > 0) {
            str[letterCount] = current[0];
            char* temp = realloc(str, (letterCount+2)*sizeof(char));
            if(temp == NULL){
                free(str);
                checkMemError(temp);
            }
            else{
                str = temp;
                letterCount++;
                //current = getchar();
                state = read(0, current, 1);
                checkError(state);
            }
        }
        if (letterCount > 0) {
            str[letterCount] = ' ';
        }
        while (current[0] == ' ') {
            //current = getchar();
            state = read(0, current, 1);
            checkError(state);
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
    /*for(i = 0; i < wordCount; i++){
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
     }*/
    for(i = 0; i < wordCount; i++){
        size_t size = 0;
        for(j = 0;; j++){
            size++;// gets the length of the word
            if(words[i][j] == ' '){//end of word
                break;
            }
        }
        if(write(1, words[i], size) == 0){
            fprintf(stderr, "Error writing file");
	    exit(1);
        }
    }
    freeWords(wordCount, words);
	fprintf(stderr, "%d", compare);
    exit(0);
}



