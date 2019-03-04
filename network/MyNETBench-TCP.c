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

#define CAPACITY 1073741824
//long CAPACITY=1073741824;

//FILL ME IN

struct thread_arg
{
	int sock;
	int buffer_size;
	int num_thread;
	char type;
};


void *tcpOpData(void *arg){
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
    int rc;
	char * buffer = (char *)malloc(sizeof(char) * thr_arg -> buffer_size);
	memset(buffer, 0, sizeof(char) * thr_arg -> buffer_size);
	long LOOPS;
	LOOPS=CAPACITY/((thr_arg -> buffer_size)*(thr_arg ->num_thread));
	//printf("newfd in the loop%d\n",thr_arg->sock);


    long i;
	for (i = 0; i < LOOPS; ++i)
	{
	    if(thr_arg ->type=='S'){
            rc=recv(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0);
            //printf("serve is receiving data %li\n",LOOPS);
            if (rc == 0) {
                printf("Connection closed at receive!\n");
                break;
            }
            if (rc < 0) {
                printf("Could not receive package!\n");
                break;
            }
            printf("%s", buffer);
            rc = send(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0);
            //printf("serve is send data: %s\n",buffer);
            if (rc < 0) {
                printf("Could not send package!\n");
                break;
            }
	    }
	    else if(thr_arg ->type=='C'){
            rc = send(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0);
            //printf("client is sending data %li\n",i);
            if (rc < 0) {
                printf("Could not send package!\n");
                break;
            }
            rc = recv(thr_arg -> sock, buffer, thr_arg -> buffer_size, 0);
            //printf("client is receiving data %s\n",buffer);
            if (rc == 0) {
                printf("Connection closed at receive!\n");
                break;
            break;
            }
            if (rc < 0) {
                printf("Could not receive package!\n");
                break;
            }
            printf("%s", buffer);
	    }
	}


	free(buffer);
	pthread_exit(NULL);


}

void *tcpClient(int BUFF_SIZE, int NUM_THREADS) {
    //int loop=1024*1024*1024/BUFF_SIZE/NUM_THREADS;
    int sockfd, rc;
    struct addrinfo hints, *res;
    socklen_t addrlen;
    struct sockaddr_storage clt;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    rc = getaddrinfo("127.0.0.1", "11155", &hints, &res);
    if (rc != 0) {
        printf("Could not get address information!\n");
        exit(-1);
    }

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    long t;
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(t=0; t<NUM_THREADS; t++) {
		struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        //printf("sockfd value is %d\n",sockfd);
        if (sockfd < 0) {
            printf("Could not create socket!\n");
            exit(-1);
        }
        rc = connect(sockfd, res->ai_addr, res->ai_addrlen);
        //printf("client rc is %d\n",rc);
        if (rc < 0) {
            printf("Could not connect to server!\n");
            exit(-1);
        }
		thr_arg -> sock = sockfd;
		thr_arg -> buffer_size = BUFF_SIZE;
		thr_arg -> num_thread = NUM_THREADS;
		thr_arg -> type='C';
        rc = pthread_create(&thread[t], &attr, tcpOpData, thr_arg);
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
    printf("client send data success!\n");
    pthread_exit(NULL);
    close(sockfd);
    freeaddrinfo(res);
}



void tcpServer(int BUFF_SIZE, int NUM_THREADS){
    int sockfd, newfd, rc;
    char buffer[BUFF_SIZE];
    struct addrinfo hints, *res;
    socklen_t addrlen;
    struct sockaddr_storage clt;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    rc = getaddrinfo(NULL, "11155", &hints, &res);
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
    //tcpClient(BUFF_SIZE, NUM_THREADS);
    if (listen(sockfd, 10) == -1) {
        printf("Could not listen to socket!");
        close(sockfd);
        exit(-1);
    }
    printf("service is listening\n");


    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    long t;
    struct timeval startTime, endTime;
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    gettimeofday(&startTime, NULL);


    for(t=0; t<NUM_THREADS; t++) {
        newfd = accept(sockfd, (struct sockaddr *) &clt, &addrlen);
        if (newfd < 0) {
            printf("Could not accept client!\n");
            close(sockfd);
        }
        printf("Service accept the client!\n");

		struct thread_arg * thr_arg = (struct thread_arg *)malloc(sizeof(struct thread_arg));
		thr_arg -> sock = newfd;
		thr_arg -> buffer_size = BUFF_SIZE;
		thr_arg -> num_thread = NUM_THREADS;
		thr_arg -> type='S';
        rc = pthread_create(&thread[t], &attr, tcpOpData, thr_arg);
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
    printf("service receive success!!\n");

    gettimeofday(&endTime, NULL);
    double exec_time = 1* (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0; // total time in ms
    FILE * fp;
    /* open the file for writing*/
    fp = fopen ("result.txt","a");
    /* write 10 lines of text into the file stream*/

    fprintf (fp, "TCP With %d threads, the BLOCK SIZE is %10d, time is %f\n", NUM_THREADS, BUFF_SIZE,exec_time);

    /* close the file*/
    fclose (fp);
    pthread_exit(NULL);
    freeaddrinfo(res);
    close(newfd);
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
            int i;
            i=0;
            while (fgets (str, sizeof str, fptr)!=NULL)
            {
                strcpy(a[i],str);
                i+=1;
                //ch = fgetc(fptr);
            }



            int NUM_THREADS = atoi(a[2]);
            int BLOCK_SIZE=atol(a[1]);

            if(BLOCK_SIZE!=1){
                BLOCK_SIZE=1024*BLOCK_SIZE/1000;
            }

            //printf("block size is%li\n",BLOCK_SIZE);
            if (a[0][0]=='T') {
                if(strcmp(argv[0],"./tcp_server")==0){
                    tcpServer(BLOCK_SIZE, NUM_THREADS);
                }
                else if(strcmp(argv[0],"./tcp_client")==0){
                    tcpClient(BLOCK_SIZE, NUM_THREADS);
                }

            }

            fclose(fptr);

        }

        closedir(d);
    }

    return 0;
}
