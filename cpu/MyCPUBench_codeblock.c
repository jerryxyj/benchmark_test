#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <dirent.h>




//FILL ME IN
const int OPERATION = 1e8;


void * QP(void *t){
    int *LOOPS = (int*)t;
    int i;
    char sum = "0";

    for (i = 0; i < *LOOPS; i++) {
        // float addition
        sum += (char)1;
    }

    return NULL;





    pthread_exit((void*) t);
}


void * HP(void *t){
    int *LOOPS = (int*)t;
    short sum = 0;
    int i;
    for (i = 0; i < *LOOPS; i++) {
        // float addition
        sum += (short)1;
    }

    return NULL;





    pthread_exit((void*) t);

}


void * SP(void *t){
    int *LOOPS = (int*)t;
    int sum = 0;
    int i;
    for (i = 0; i < *LOOPS; i++) {
        // float addition
        sum += 1;
    }

    return NULL;





    pthread_exit((void*) t);
}

void * DP(void *t){
    int *LOOPS = (int*)t;
    float sum = 0;
    int i;
    for (i = 0; i < *LOOPS; i++) {
        // float addition
        sum += (float)1;
    }

    return NULL;





    pthread_exit((void*) t);
}

void* calculate_QP(int NUM_THREADS){

/*Below code came from Pthread tutorial:https://computing.llnl.gov/tutorials/pthreads/
It used create multiple thread and joint thread blocks the calling thread until the specified threadid thread terminates.



*/

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    struct timeval startTime, endTime;
    void *status;
    int loop;
    loop=OPERATION/NUM_THREADS;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&startTime, NULL);

    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_create(&thread[t], &attr, QP, &loop);
       if (rc) {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
          }
       }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_join(thread[t], &status);
       if (rc) {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
          }
       }
    gettimeofday(&endTime, NULL);
    double exec_time = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    printf("With %d threads, the execution time is %10f ms\n", NUM_THREADS, exec_time);
//pthread_exit(NULL);



}

void* calculate_HP(int NUM_THREADS){
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    struct timeval startTime, endTime;
    void *status;
    int loop;
    loop=OPERATION/NUM_THREADS;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&startTime, NULL);

    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_create(&thread[t], &attr, HP, &loop);
       if (rc) {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
          }
       }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_join(thread[t], &status);
       if (rc) {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
          }
       }
    gettimeofday(&endTime, NULL);
    double exec_time = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    printf("With %d threads, the execution time is %10f ms\n", NUM_THREADS, exec_time);
//pthread_exit(NULL);





}

void* calculate_SP(int NUM_THREADS){
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    struct timeval startTime, endTime;
    void *status;
    int loop;
    loop=OPERATION/NUM_THREADS;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&startTime, NULL);

    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_create(&thread[t], &attr, SP, &loop);
       if (rc) {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
          }
       }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_join(thread[t], &status);
       if (rc) {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
          }
       }
    gettimeofday(&endTime, NULL);
    double exec_time = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    printf("With %d threads, the execution time is %10f ms\n", NUM_THREADS, exec_time);
//pthread_exit(NULL);



}


void* calculate_DP(int NUM_THREADS){
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    struct timeval startTime, endTime;
    void *status;
    int loop;
    loop=OPERATION/NUM_THREADS;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&startTime, NULL);

    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_create(&thread[t], &attr, DP, &loop);
       if (rc) {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
          }
       }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_join(thread[t], &status);
       if (rc) {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
          }
       }
    gettimeofday(&endTime, NULL);
    double exec_time = 1000.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // total time in ms
    printf("With %d threads, the execution time is %10f ms\n", NUM_THREADS, exec_time);
//pthread_exit(NULL);


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
            char a[2][100];
            char str[100];
            fptr = fopen(dir->d_name, "r");
            int i;
            i=0;
            while (fgets (str, 100, fptr)!=NULL)
            {
                strcpy(a[i],str);
                i+=1;
                //ch = fgetc(fptr);
            }



            int NUM_THREADS = atoi(a[1]);

            if (strcmp(a[0], "DP") == 0) {
                calculate_DP(NUM_THREADS);
            }
            else if (strcmp(a[0], "HP") == 0){
                calculate_HP(NUM_THREADS);
            }
            else if (strcmp(a[0], "QP") == 0){
                calculate_QP(NUM_THREADS);
            }
            else if (strcmp(a[0], "SP") == 0){
                calculate_SP(NUM_THREADS);
            }
            fclose(fptr);

        }

        closedir(d);
    }

    return(0);



}



