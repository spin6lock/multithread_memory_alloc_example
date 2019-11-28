#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 
#include <string.h>  // header for memset
#define _GNU_SOURCE
#include <fcntl.h>   // O_* constant
  
#define PAIR_SIZE 32
#define SIZE 1024 * 1024 * 4
#define ABORT_COUNT 20

int pipefds[PAIR_SIZE][2];
void *producer(void *i) { 
    int index = (int)i;
    int pipefd = pipefds[index][1];
    int *ptr = NULL;
    int size = 0;
    int counter = 0;
    while (1) {
        sleep(1); 
        size = random() % SIZE;
        counter++;
        if (counter < ABORT_COUNT) {
            printf("Producer Thread malloc size: %d\n", size); 
            ptr = malloc(size);
            memset(ptr, 1, size);
            write(pipefd, &ptr, sizeof(ptr));
        }
        else {
            printf("Producer:%d finish\n", index); 
            return NULL;
        }
    }
    return NULL; 
} 

void *consumer(void *i) {
    int index = (int)i;
    int pipefd = pipefds[index][0];
    int mess = 1;
    int counter = 0;
    int *ptr = NULL;
    while (1) {
        sleep(1); 
        counter++;
        if (counter < ABORT_COUNT) {
            printf("Consumer Thread:%d\n", index); 
            read(pipefd, &ptr, sizeof(ptr));
            mess = mess + *ptr;
            free(ptr);
        } else {
            printf("Consumer:%d return\n", index); 
            return NULL;
        }
    }
    return NULL; 
}
   
int main() { 
    pthread_t consumer_thread_ids[PAIR_SIZE]; 
    pthread_t producer_thread_ids[PAIR_SIZE]; 
    for (int i=0;i<PAIR_SIZE; i++) {
        if (pipe(pipefds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        printf("pass in index:%d\n", i);
        pthread_create(&consumer_thread_ids[i], NULL, consumer, (void*)i); 
        pthread_create(&producer_thread_ids[i], NULL, producer, (void*)i); 
    }
    for (int i=0;i<PAIR_SIZE;i++) {
        pthread_join(consumer_thread_ids[i], NULL); 
        pthread_join(producer_thread_ids[i], NULL); 
    }
	return 0;
}

