/* Client code */
/* TODO : Modify to meet your need */
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>

#define BUFFER_SIZE BUFSIZ
//#define PORT_NUMBER     5000
 
//#define FILENAME        "/home/pawel/Documents/cat_send.jpg"

int main(int argc, char **argv)
{
        int buffsize = BUFFER_SIZE;

        int client_socket;
        ssize_t len;
        struct sockaddr_in remote_addr;
        char buffer[BUFSIZ];
        off_t file_size;
        FILE *received_file;
        long remain_data = 0;
        char *PORT_NUMBER;
        char *FILENAME;
        char *SERVER_ADDRESS;
        long dataRetrieved;
        char *fileToSave;
        time_t s,e;

        printf("%d\n", buffsize);

        if(argc < 3)
        {
            printf("Proper usage:server <SERVER_ADDRESS> <PORT_NUMBER> <FILENAME>\n");
            exit(-1);
        }

        SERVER_ADDRESS =  argv[1];
        PORT_NUMBER = argv[2];        
        FILENAME = argv[3];

        /* Zeroing remote_addr struct */
        memset(&remote_addr, 0, sizeof(remote_addr));

        /* Construct remote_addr struct */
        remote_addr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_ADDRESS, &(remote_addr.sin_addr));
        remote_addr.sin_port = htons(atoi(PORT_NUMBER));

        /* Create client socket */
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
                fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Connect to the server */
        if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
        {
                fprintf(stderr, "Error on connect --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Receiving file size */
        recv(client_socket, &file_size, sizeof(off_t), 0);
        //file_size = atoi(buffer);
        //fprintf(stdout, "\nFile size : %d\n", file_size);

        received_file = fopen(FILENAME, "w");
        if (received_file == NULL)
        {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        remain_data = file_size;
        
        fileToSave = new char[remain_data];
        //printf("%d\n",BUFSIZ);
        //printf("%d\n",recv(client_socket, buffer, BUFSIZ, 0));

        struct timeval stop, start;
        gettimeofday(&start, NULL);
        dataRetrieved = 0;
        while (((len = recv(client_socket, buffer, buffsize, 0)) > 0) )
        {
                memcpy(fileToSave + dataRetrieved,buffer, len);
                //fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                dataRetrieved += len;
                //fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
        }
        gettimeofday(&stop, NULL);

	double elapsedTime = (stop.tv_sec - start.tv_sec); //sec
	elapsedTime += (stop.tv_usec - start.tv_usec) / 1000000.0; //usec to sec
        
	fwrite(fileToSave, sizeof(char), dataRetrieved, received_file);
        fclose(received_file);

    double mb = (dataRetrieved/ 1000000.0) /elapsedTime ;

	//fprintf(stdout,"TIME::%lf\n",elapsedTime);
        fprintf(stdout, "RECEIVED:%ld:TIME:%lf:REMAIN:%ld:MBReceived:%.3lf:MB/s:%.3lf\n", dataRetrieved, elapsedTime, (long)remain_data, dataRetrieved/(double)1000000 , mb);

        close(client_socket);

        return 0;
}
