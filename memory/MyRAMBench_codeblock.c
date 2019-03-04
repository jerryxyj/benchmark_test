#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

//FILL ME IN


void *sequential_access(void *n){


    int *m = (int*)n; // m is a point variable
    int realsize;
    long size =1024 * 1024 * 1024;
    int loop = 0; // initiate loop
    if (*m == 1){
            realsize=1;
            loop = 1000000000;
    }

    else if(*m == 1000){

            realsize=1024;
            loop = 1000000;
    }
    else if(*m == 1000000){
            realsize=1024*1024;
            loop = 1000;
    }

    else{
            realsize=1024*1024*10;
            loop = 100;
    }
    printf("haha%ld",size);
    char *des;
    des = (char *)malloc(size * sizeof(char)); // give size byte(char) to destination
    char *sor;
    if(des!=NULL){
        printf("success");
    }
    sor = (char *)malloc(size * sizeof(char)); // give size byte(char) to source


   // int mem = 0, i = 0;
    memcpy(des, sor, 1024*1024);
    /*
    for (i = 0; i < loop; i++) {
        memcpy(des + mem, sor + mem, realsize);//copy (*m)byte from sor+mem to des+mem
        mem += realsize;
    }*/

    free(des); //free memory
    free(sor);
    return NULL;
}

void *random_access(void *n){
    int *m = (int*)n; // m is a point variable
    int realsize;
    unsigned long size = 2ul*1024ul * 1024ul * 1024ul;
    int loop = 0; // initiate loop
    if (*m == 1){
            realsize=1;
            loop = 1000000000;
    }

    else if(*m == 1000){
            realsize=1024;
            loop = 1000000;
    }
    else if(*m == 1000000){
            realsize=1024*1024;
            loop = 1000;
    }

    else{
            realsize=1024*1024*10;
            loop = 100;
    }

    char *des = (char *)malloc(size * sizeof(char)); // give size byte(char) to destination
    char *sor = (char *)malloc(loop * realsize * sizeof(char)); // give size byte(char) to source
    int mem = 0, i = 0;
    for (i = 0; i < loop; i++) {
            int index_1 = rand()%(size - realsize - 1); //random index_1
            int index_2 = rand()%(loop * realsize - realsize - 1); //random index_2
            memcpy(des + index_1, sor + index_2, realsize);
    }
    free(des); //free memory
    free(sor);
    return NULL;




}


int main(int argc, char *argv[]) {
    // argv[0] cpu_benchmark
    // argv[1] DP / HP/QP/SP
    // 1thread / 2thread / 4thread / 8thread
    int NUM_THREADS =1;
    //int BLOCK_SIZE=1000;

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    struct timeval startTime, endTime;
    void *status;




/*

    if (strcmp(argv[1], "RWR") == 0) {

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        gettimeofday(&startTime, NULL);

        for(t=0; t<NUM_THREADS; t++) {
            rc = pthread_create(&thread[t], &attr, sequential_access, &BLOCK_SIZE);
            if (rc) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
        pthread_attr_destroy(&attr);
        for(t=0; t<NUM_THREADS; t++) {
            rc = pthread_join(thread[t], &status);
            if (rc) {
                printf("ERROR; return code from pthread_join() is %d\n", rc);
                exit(-1);
            }
        }
        gettimeofday(&endTime, NULL);// get ending time
        //double latency = (1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0) / loop / byte;
        //double throughput = (i * byte / (1024.0 * 1024.0)) / (latency * byte / 1000.0);
        //printf("Using %d thread(s), sequential access %10d bytes, the throughput is %10f MB/sec, the latency is %10.9f ms\n", i, byte,
    }
    else if (strcmp(argv[1], "RWS") == 0){
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        gettimeofday(&startTime, NULL);

        for(t=0; t<NUM_THREADS; t++) {
            rc = pthread_create(&thread[t], &attr, sequential_access, &BLOCK_SIZE);
            if (rc) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }

        pthread_attr_destroy(&attr);
        for(t=0; t<NUM_THREADS; t++) {
            rc = pthread_join(thread[t], &status);
            if (rc) {
                printf("ERROR; return code from pthread_join() is %d\n", rc);
                exit(-1);
            }
        }
        gettimeofday(&endTime, NULL);// get ending time
        //double latency = (1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0) / loop / byte;
        //double throughput = (i * byte / (1024.0 * 1024.0)) / (latency * byte / 1000.0);
        //printf("Using %d thread(s), sequential access %10d bytes, the throughput is %10f MB/sec, the latency is %10.9f ms\n", i, byte,

    }
*/


        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        gettimeofday(&startTime, NULL);
        int BLOCK_SIZE1=1000;



        for(t=0; t<1; t++) {

            rc = pthread_create(&thread[t], &attr, sequential_access, &BLOCK_SIZE1);
            if (rc) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }

    /* Free attribute and wait for the other threads */
        pthread_attr_destroy(&attr);
        for(t=0; t<1; t++) {
            rc = pthread_join(thread[t], &status);
            if (rc) {
                printf("ERROR; return code from pthread_join() is %d\n", rc);
                exit(-1);
            }
        }
        gettimeofday(&endTime, NULL);// get ending time
        double latency = (1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0);
        //double throughput = (i * byte / (1024.0 * 1024.0)) / (latency * byte / 1000.0);
        printf("Using %d thread(s),%f/n",NUM_THREADS,latency);
    return 0;
}


