#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc != 3){
        fprintf(stderr, "Must input 2 arguments!");
        exit(1);
    }
    char* from = argv[1];
    char* to = argv[2];
    if(strlen(from) != strlen(to)){
        fprintf(stderr, "Must be same character length!");//Error if the inputs are not the same length
        exit(1);
    }
    int i;
    int j;
    for(i = 0; i < strlen(from); i++){
        for(j = i+1; j < strlen(from); j++){
            if(from[i] == from[j]){
                fprintf(stderr, "Duplicate characters are not allowed!");
                exit(1);
            }
        }
    }
    int put = 0;
    char current = getchar(); //char array
    while(current != EOF){
        int i;
        for(i = 0; i < strlen(from); i++){
            if(current == from[i]){
                put = 1;
                putchar(to[i]);
                break;
            }
            else {
                put = 0;
            }
        }
        if(put == 0){
            putchar(current);
        }
        current = getchar();
    }
    return 0;
}



