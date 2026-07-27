#include "Lexical Analyzer/lexical_analyzer.h"
#include <stdio.h>
#include <string.h>
#include <string.h>

int check_file_type(const char *filename){
    const char *dot = strrchr(filename, '.');
    return (dot && strcmp(dot, ".atl") == 0);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "ERROR: No atl file provided!\n Usage: %s <file.atl>\n", argv[0]);
        return 1;
    }
    if(!check_file_type(argv[1])){
        fprintf(stderr, "ERROR: Unknown file type %s\n Expected file type: \".atl\"\n", argv[1]);
        return 1;
    }
    //for now, only 1 atl file can be executed. It will change.
    parse_file(argv[1]);
}