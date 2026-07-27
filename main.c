#define _POSIX_C_SOURCE 200809L
#include "Lexical Analyzer/lexical_analyzer.h"
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <time.h>

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
    
    //initialize the tables
    initialize_char_table();
    initialize_hash_table();

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    //for now, only 1 atl file can be executed. It will change.
    parse_file(argv[1]);

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Elapsed time calculation
    double seconds = (end.tv_sec - start.tv_sec) + 
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    // %.9f prints 9 decimal places (exact nanoseconds)
    fprintf(stderr, "\nLex time: %.9f seconds (%.3f µs / %.0f ns)\n", 
            seconds, seconds * 1e6, seconds * 1e9);

    return 0;
}