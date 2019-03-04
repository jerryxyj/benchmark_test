#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <dirent.h>


#define RUN 2
//FILL ME IN
int Thread=0;


void *sequential_access(void *n){


    int *m = (int*)n; // m is a point variable
    int realsize;
    long size =1024 * 1024 * 1024;
    int loop = 0; // initiate loop
    if (*m == 1){
        realsize=1;
        loop = 1024*1024*1024/Thread;
    }

    else if(*m == 1000){
        realsize=1024;
        loop = 1024*1024/Thread;
    }
    else if(*m == 1000000){
        realsize=1024*1024;
        loop = 1024/Thread;
    }

    else{
        realsize=1024*1024*10;
        loop = (int)102.4/Thread;
    }
    char *des;
    des = (char *)malloc(size * sizeof(char)); // give size byte(char) to destination
    //printf("%x",des);
    char *sor;
    sor = (char *)malloc(size * sizeof(char)); // give size byte(char) to source


    int mem = 0, i = 0;

     for (i = 0; i < loop; i++) {
     memcpy(des + mem, sor + mem, realsize);//copy (*m)byte from sor+mem to des+mem
     mem += realsize;
     }

    free(des); //free memory
    free(sor);
    pthread_exit((void*) n);
}

void *random_access(void *n){

    int *m = (int*)n; // m is a point variable
    int realsize;
    long size =1024*1024*1024;
    int loop = 0; // initiate loop
    if (*m == 1){
        realsize=1;
        loop = 1024*1024/Thread;
    }

    else if(*m == 1000){
        realsize=1024;
        loop = 1024*1024/Thread;
    }
    else if(*m == 1000000){
        realsize=1024*1024;
        loop = 1024/Thread;
    }

    else{
        realsize=1024*1024*10;
        loop  = (int)102.4/Thread;
    }

    char *des = (char *)malloc(size * sizeof(char)); // give size byte(char) to destination
    char *sor = (char *)malloc(size * sizeof(char)); // give size byte(char) to source
    //int mem = 0, i = 0;
    int i;
    for (i = 0; i < loop; i++) {
        int index_1 = rand()%(size - realsize - 1); //random index_1
        int index_2 = rand()%(size - realsize - 1); //random index_2
        memcpy(des + index_1, sor + index_2, realsize);
    }
    free(des); //free memory
    free(sor);
    pthread_exit((void*) n);






}


int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    d = opendir("./data");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            FILE *fptr;
            char a[3][10];
            char str[128];
            fptr = fopen(dir->d_name, "r");
            int i;
            i=0;
            while (fgets (str, sizeof str, fptr)!=NULL)
            {
                strcpy(a[i],str);
                i+=1;
                //ch = fgetc(fptr);
            }
            fclose(fptr);



            int NUM_THREADS = atoi(a[2]);
            Thread=NUM_THREADS;
            int BLOCK_SIZE=atoi(a[1]);
            pthread_t thread[NUM_THREADS];
            pthread_attr_t attr;
            int rc;
            long t;
            struct timeval startTime, endTime;
            void *status;

             if (a[0][2]=='S') {
                 gettimeofday(&startTime, NULL);
                for(int i=0;i<RUN;i++){
                 pthread_attr_init(&attr);
                 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


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
                }
                 gettimeofday(&endTime, NULL);// get ending time
                 double latency = (1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0) / 1073741824/RUN;
                 double throughput = (NUM_THREADS * BLOCK_SIZE / (1024.0 * 1024.0)) / (latency * BLOCK_SIZE / 1000.0);
                 FILE * fp;
                   /* open the file for writing*/
                 fp = fopen ("result.txt","a");

                   /* write 10 lines of text into the file stream*/


                 fprintf (fp,"Using %d thread(s), random access %10d bytes, the throughput is %10f MB/sec, the latency is %10.9f us\n", NUM_THREADS,BLOCK_SIZE,throughput, latency*1000);

                   /* close the file*/
                 fclose (fp);
                 printf("Using %d thread(s), sequential access %10d bytes, the throughput is %10f MB/sec, the latency is %10.9f us\n", NUM_THREADS,BLOCK_SIZE,throughput, latency*1000);
                 }
                 else if (a[0][2]=='R'){
                 gettimeofday(&startTime, NULL);
                for(int i=0;i<RUN;i++){
                 pthread_attr_init(&attr);
                 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


                 for(t=0; t<NUM_THREADS; t++) {
                 rc = pthread_create(&thread[t], &attr, random_access, &BLOCK_SIZE);
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
                }
                 gettimeofday(&endTime, NULL);// get ending time
                 double latency = (1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0) / 1073741824/RUN;
                 double throughput = (NUM_THREADS * BLOCK_SIZE / (1024.0 * 1024.0)) / (latency * BLOCK_SIZE / 1000.0);
                 FILE * fp;
                   /* open the file for writing*/
                 fp = fopen ("result.txt","a");

                   /* write 10 lines of text into the file stream*/


                 fprintf (fp,"Using %d thread(s), random access %10d bytes, the throughput is %10f MB/sec, the latency is %10.9f us\n", NUM_THREADS,BLOCK_SIZE,throughput, latency*1000);

                   /* close the file*/
                 fclose (fp);
                 printf("Using %d thread(s), random access %10d bytes, the throughput is %10f MB/sec, the latency is %10.9f us\n", NUM_THREADS,BLOCK_SIZE,throughput, latency*1000);

                }



        }

        closedir(d);

}


/*

            int NUM_THREADS = 2;
	    Thread=NUM_THREADS;
		int BLOCK_SIZE=1;
	    pthread_t thread[NUM_THREADS];
	    pthread_attr_t attr;
	    int rc;
	    long t;
	    struct timeval startTime, endTime;
	    void *status;
	     pthread_attr_init(&attr);
	     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	     gettimeofday(&startTime, NULL);

	     for(t=0; t<NUM_THREADS; t++) {
	     rc = pthread_create(&thread[t], &attr, random_access, &BLOCK_SIZE);
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
	    double exec_time = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0; // total time in ms
	    printf("sequential and block size is %d,With %d threads, the execution time is %10f ms\n", BLOCK_SIZE,NUM_THREADS, exec_time);



*/

    return 0;
}
