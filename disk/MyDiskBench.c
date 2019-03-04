#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>



int Thread=0;
long long capacity;


//FILL ME IN


void * WriteFirst(int block_size)
{   long long filesize=10737418240;
   // int block_size=1024 * 1024*100;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    int fin;
    // create file / write file
    if ((fin = open("test.bin", O_CREAT|O_TRUNC|O_WRONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    int i = 0;
    memset(buffer, '0', block_size);
    for (i = 0; i < filesize / block_size; i++) {
        // set buffer content to be written
        // write to disk sequentially
        write(fin, buffer, block_size);
    }
    fsync(fin);
    close(fin);
    free(buffer);

    return NULL;
}

void * Read_Random(void * arg)
{
    int file_position = 0;
    int block_size = (int)(long)arg;
//	printf("block size is%d\n",block_size);
    char * buffer = (char *)malloc(block_size * sizeof(char));
    srand((int)time(0));
    int fin;
    // Open the file for READ only.
    if ((fin = open("test.bin", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }
    int i = 0;
    for (i = 0; i < capacity / (block_size*Thread); i++) {
        // random position in the file
        file_position = (rand() % (capacity / block_size)) * block_size;
        // locate to the random position
        lseek(fin, file_position, SEEK_SET);
        // read file
        read(fin, buffer, block_size);
    }
    fsync(fin);
    close(fin);
    free(buffer);

    return NULL;
}

void * Read_Sequential(void * arg)
{
    int block_size = (int)(long)arg;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    int fin;
    // read-only
    if ((fin = open("test.bin", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }
    int i = 0;
    for (i = 0; i < capacity / (block_size*Thread); i++) {
        // read the file sequentially
        read(fin, buffer, block_size);
    }
    fsync(fin);
    close(fin);
    free(buffer);
    return NULL;
}


void * Write_Random(void * arg)
{
    int file_position = 0;
    int block_size = (int)(long)arg;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    srand((int)time(0));
    int fin;
    // create file / write file
    if ((fin = open("test.bin", O_CREAT|O_WRONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    int i = 0;
    memset(buffer, '0', block_size);
    for (i = 0; i < capacity / (block_size*Thread); i++) {
        // random position in the file
        file_position = (rand() % (capacity / block_size)) * block_size;
        // set the content to be written
        // find the random position in file
        lseek(fin, file_position, SEEK_SET);
        // write from buffer into file
        write(fin, buffer, block_size);
    }
    fsync(fin);
    close(fin);
    free(buffer);

    return NULL;
}



void * Write_Sequential(void * arg)
{
    int block_size = (int)(long)arg;
    char * buffer = (char *)malloc(block_size * sizeof(char));
    int fin;
    // create file / write file
    if ((fin = open("test.bin", O_CREAT|O_WRONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    int i = 0;
    memset(buffer, '0', block_size);
    for (i = 0; i < capacity / (block_size*Thread); i++) {
        // set buffer content to be written
        // write to disk sequentially
        write(fin, buffer, block_size);
    }
    fsync(fin);
    close(fin);
    free(buffer);

    return NULL;
}




int main(int argc, char **argv)
{
    DIR *d;
    struct dirent *dir;
    d = opendir("./data");
    WriteFirst(1024*1024*100);

    if (d)
    {
        while ((dir = readdir(d)) != NULL&&sizeof(dir->d_name)>2)
        {
                FILE *fptr;
                char a[3][10];
                char str[128];
                fptr = fopen(dir->d_name, "r");
                int i;
                i=0;
                while (fgets (str, sizeof str, fptr)!=NULL&&sizeof(dir->d_name)>2)
                {
                    strcpy(a[i],str);
                    i+=1;
                    //ch = fgetc(fptr);
                }
                fclose(fptr);
                int NUM_THREADS = atoi(a[2]);
                Thread=NUM_THREADS;
                long BLOCK_SIZE=atoi(a[1]);
                long block_size;
                if (BLOCK_SIZE==1) {
                    block_size = 1;
                }
                else if (BLOCK_SIZE==1000){
                    block_size = 1024;
                }
                else if (BLOCK_SIZE==10000){
                    block_size = 1024 * 1024*10;
                }
                else if (BLOCK_SIZE==100000){
                    block_size = 1024 * 1024*100;
                }
                else{
                    printf("The block size is not correct.\n");
                    continue;
                }

                if(BLOCK_SIZE!=1){
                    capacity=10737418240;
                }
                else{
                    capacity=1073741824;
                }
                pthread_t thread[NUM_THREADS];
                pthread_attr_t attr;
                int rc;
                long t;
                struct timeval startTime, endTime;
                void *status;

                 gettimeofday(&startTime, NULL);
                 pthread_attr_init(&attr);
                 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


                 for(t=0; t<NUM_THREADS; t++) {
                        if (a[0][0]=='R'&&a[0][1]=='R') {
                        rc = pthread_create(&thread[t], &attr, Read_Random, (void *)(long)block_size);
                        }
                        else if(a[0][0]=='R'&&a[0][1]=='S'){
                        rc = pthread_create(&thread[t], &attr, Read_Sequential, (void *)(long)block_size);
                        }
                        else if(a[0][0]=='W'&&a[0][1]=='R'){
                        rc = pthread_create(&thread[t], &attr, Write_Random, (void *)(long)block_size);
                        }
                        else if(a[0][0]=='W'&&a[0][1]=='S'){
                        rc = pthread_create(&thread[t], &attr, Write_Sequential, (void *)(long)block_size);
                        }
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
                    double execTime = 1.0 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0; // total time in ms
                    double throughput = ((double) capacity / (1024.0 * 1024.0)) / (execTime); // MBps
                    double latency = execTime *block_size*1000/ capacity; //
                    double IOPS=1024*1024/execTime;
                 FILE * fp;
                   // open the file for writing
                 fp = fopen ("result.txt","a");

                   // write 10 lines of text into the file stream*

                    if(BLOCK_SIZE!=1){
                        fprintf (fp,"Using %d thread(s), %c %c _access %10li MB, the throughput is %10f MB/sec\n", NUM_THREADS,a[0][0],a[0][1],BLOCK_SIZE/1000,throughput);
                        //printf ("Using %d thread(s), %c %c _access %10li MB, the throughput is %10f MB/sec, the latency is %10.9f us\n", NUM_THREADS,a[0][0],a[0][1],BLOCK_SIZE/1000,throughput, latency);
                    }
                    else{
                        fprintf (fp,"Using %d thread(s), %c %c _access %10li KB, the IOPS is %10f, the latency is %10.9f ms\n", NUM_THREADS,a[0][0],a[0][1],BLOCK_SIZE,IOPS, latency);
                        //printf ("Using %d thread(s), %c %c _access %10li KB, the throughput is %10f MB/sec, the latency is %10.9f us\n", NUM_THREADS,a[0][0],a[0][1],BLOCK_SIZE,throughput, latency);
                    }
                   // close the file*
                 fclose (fp);
                 //printf("Using %d thread(s), %c _access %10d bytes, the throughput is %10f GB/sec, the latency is %10.9f us\n", NUM_THREADS,a[0][2],BLOCK_SIZE,throughput, latency);

                 }






        closedir(d);

}





    return 0;
}
