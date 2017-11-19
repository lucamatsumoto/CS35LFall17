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
    int k;
    int put = 0;
    char current[1]; //char array
    ssize_t check = read(0, current, 1); //file descriptor 0 stands for stdin, read 1 byte at a time
    while(check > 0){
        for(k = 0; k < strlen(from); k++){
            if(current[0] == from[k]){
                current[0] = to[k];
                write(1, current, 1);
                put = 1;
                break;
            }
        }
        if(put == 0){
            write(1, current, 1);
        }
        check = read(0, current, 1);
    }
    return 0;
}
