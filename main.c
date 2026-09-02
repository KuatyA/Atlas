#define _POSIX_C_SOURCE 200809L
#include "Lexical Analyzer/lexical_analyzer.h"
#include <stdio.h>
#include <time.h>

 FileQueue queue;

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

    //start making queue
    queue.files = &argv[1];
    queue.total_files = argc - 1;
    queue.next_file_index = 0;
    pthread_mutex_init(&queue.lock, NULL);
    
    TokenStruct token_struct[queue.total_files];

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for(int i = 0; i < NUM_THREADS; i++){
        thread_ids[i] = i;
        if(pthread_create(&threads[i], NULL, worker_thread, &thread_ids[i]) != 0){
            perror("ERROR: Couldn't create thread.");
            return 1;
        }
    }
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    pthread_mutex_destroy(&queue.lock);

    // Elapsed time calculation
    double seconds = (end.tv_sec - start.tv_sec) + 
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    // %.9f prints 9 decimal places (exact nanoseconds)
    fprintf(stderr, "\nLex time: %.9f seconds (%.3f µs / %.0f ns)\n", 
            seconds, seconds * 1e6, seconds * 1e9);

    return 0;
}