#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <dirent.h>

#define CAPACITY 1024*1024*1024
//FILL ME IN

struct thread_arg
{
	int sock;
	int buffer_size;
	int num_thread;
	struct addrinfo *add_info;
	char type;
//    struct sockaddr_storage addr;
};


void *udpOpData(void *arg){
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
    int rc;
	char * buffer = (char *)malloc(sizeof(char) * thr_arg -> buffer_size);
	memset(buffer, 0, sizeof(char) * thr_arg -> buffer_size);
	long LOOPS;
	LOOPS=CAPACITY/(thr_arg -> buffer_size*thr_arg ->num_thread);
    socklen_t addrlen;
    struct sockaddr_storage clt;
    addrlen = sizeof clt;
    long i;

    for (i = 0; i < 100*LOOPS/2; ++i)
    {
        if(thr_arg ->type=='S'){
            rc=recvfrom(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0,(struct sockaddr *)&clt, &addrlen);
            if (rc < 0) {
                printf("Could not receive package!\n");
                break;
            }
            //printf("receive is %li\n",i);

            rc = sendto(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0,(struct sockaddr *)&clt, addrlen);
            if (rc < 0) {
                printf("Could not send package!\n");
                break;
            }
           // printf("send data%li\n",i);

        }

        else if(thr_arg ->type=='C'){
            rc = sendto(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0,(struct sockaddr *)(thr_arg -> add_info->ai_addr), thr_arg -> add_info->ai_addrlen);
            if (rc < 0) {
                printf("Could not send package!\n");
                break;
            }
            //printf("send data%li\n",i);

            rc=recvfrom(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0,(struct sockaddr *)(thr_arg -> add_info->ai_addr), &thr_arg -> add_info->ai_addrlen);
            if (rc < 0) {
                printf("Could not receive package!\n");
                break;
            }
            //printf("receive is %li\n",i);

        }
    }

	free(buffer);
	pthread_exit(NULL);
    //return NULL;


}



void udpServer(int BUFF_SIZE, int NUM_THREADS){
    int sockfd,rc;
    char buffer[BUFF_SIZE];
    struct addrinfo hints, *res;
    socklen_t addrlen;
    struct sockaddr_storage clt;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int t;
    struct timeval startTime, endTime;
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&startTime, NULL);

    for(t=0; t<NUM_THREADS; t++) {
    char port[5];
    sprintf(port, "%d", 11155+t);
    rc = getaddrinfo(NULL, port, &hints, &res);
    if (rc != 0) {
        printf("Could not get address information!\n");
        exit(-1);
    }
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        printf("Could not create socket!\n");
        exit(-1);
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        printf("Could not bind socked!");
        close(sockfd);
        exit(-1);
    }
    struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
    thr_arg -> sock = sockfd;
    thr_arg -> buffer_size = BUFF_SIZE;
    thr_arg -> num_thread = NUM_THREADS;
    thr_arg ->add_info=res;
    thr_arg ->type='S';

        rc = pthread_create(&thread[t], &attr, udpOpData, thr_arg);
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
        printf("server\n");
    }

    gettimeofday(&endTime, NULL);
    double exec_time = 1* (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0; // total time in ms
    FILE * fp;
    /* open the file for writing*/
    fp = fopen ("result.txt","a");
    /* write 10 lines of text into the file stream*/

    fprintf (fp, "UDP With %d threads, the BLOCK SIZE is %10d, time is %f\n", NUM_THREADS, BUFF_SIZE,exec_time);

    /* close the file*/
    fclose (fp);
    pthread_exit(NULL);

    freeaddrinfo(res);
    close(sockfd);

}



void *udpClient(int BUFF_SIZE, int NUM_THREADS) {
    int sockfd, rc;
    char buffer[BUFF_SIZE];
    struct addrinfo hints, *res;
    socklen_t addrlen;
    struct sockaddr_storage clt;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int t;
    struct timeval startTime, endTime;
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&startTime, NULL);


    for(t=0; t<NUM_THREADS; t++) {
        char port[5];
        sprintf(port, "%d", 11155+t);
        rc = getaddrinfo("127.0.0.1", port, &hints, &res);
        if (rc != 0) {
            printf("Could not get address information!\n");
            exit(-1);
        }
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) {
            printf("Could not create socket!\n");
            exit(-1);
        }
        struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
        thr_arg -> sock = sockfd;
        thr_arg -> buffer_size = BUFF_SIZE;
        thr_arg -> num_thread = NUM_THREADS;
        thr_arg ->add_info=res;
        thr_arg ->type='C';
        rc = pthread_create(&thread[t], &attr, udpOpData, thr_arg);
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
        printf("client\n");
    }
    pthread_exit(NULL);
    freeaddrinfo(res);
    close(sockfd);

}



int main(int argc, char **argv)
{
    DIR *d;
    struct dirent *dir;
    d = opendir("./data");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            FILE *fptr;
            char a[2][10];
            char str[128];
                fptr = fopen(dir->d_name, "r");
            int i ,j;
            i=0;
            while (fgets (str, sizeof str, fptr)!=NULL)
            {
                strcpy(a[i],str);
                i+=1;
                //ch = fgetc(fptr);
            }



            int NUM_THREADS = atoi(a[2]);
            int BLOCK_SIZE=atoi(a[1]);
            if(BLOCK_SIZE!=1){
                BLOCK_SIZE=1024*BLOCK_SIZE/1000;
            }


            if (a[0][0]=='U') {
                if(strcmp(argv[0],"./udp_server")==0){
                    udpServer(BLOCK_SIZE, NUM_THREADS);

                }
                else if(strcmp(argv[0],"./udp_client")==0){
                        udpClient(BLOCK_SIZE, NUM_THREADS);
                }
            }




            fclose(fptr);

        }

        closedir(d);
    }

    return 0;
}
