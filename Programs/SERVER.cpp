/* Server code */
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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/time.h>

#define BUFFER_SIZE BUFSIZ

//#define PORT_NUMBER     5000
//#define SERVER_ADDRESS  "127.0.0.1"
//#define FILE_TO_SEND    "/home/pawel/Documents/cat.jpg"

int main(int argc, char **argv)
{
        int buffsize = BUFFER_SIZE;

        int server_socket;
        int peer_socket;
        socklen_t       sock_len;
        ssize_t len;
        struct sockaddr_in      server_addr;
        struct sockaddr_in      peer_addr;
        int fd;
        int sent_bytes = 0;
        char file_size[256];
        struct stat file_stat;
        int offset;
        int remain_data;
        char *PORT_NUMBER;
        char *FILE_TO_SEND;
        char *SERVER_ADDRESS;

        if(argc < 4)
        {
            printf("Proper usage:server <PORT_NUMBER> <SERVER_ADDRESS> <FILE_TO_SEND>\n");
            exit(-1);
        }

        PORT_NUMBER = argv[1];
        SERVER_ADDRESS = argv[2];
        FILE_TO_SEND = argv[3];

        /* Create server socket */
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1)
        {
                fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Zeroing server_addr struct */
        memset(&server_addr, 0, sizeof(server_addr));
        /* Construct server_addr struct */
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_ADDRESS, &(server_addr.sin_addr));
        server_addr.sin_port = htons(atoi(PORT_NUMBER));

        /* Bind */
        if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
        {
                fprintf(stderr, "Error on bind --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Listening to incoming connections */
        if ((listen(server_socket, 5)) == -1)
        {
                fprintf(stderr, "Error on listen --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        fd = open(FILE_TO_SEND, O_RDONLY);
        if (fd == -1)
        {
                fprintf(stderr, "Error opening file --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Get file stats */
        if (fstat(fd, &file_stat) < 0)
        {
                fprintf(stderr, "Error fstat --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        //fprintf(stdout, "File Size: \n%d bytes\n", (int)file_stat.st_size);

        sock_len = sizeof(struct sockaddr_in);
        /* Accepting incoming peers */
        peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
        if (peer_socket == -1)
        {
                fprintf(stderr, "Error on accept --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }
        //fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));

        sprintf(file_size, "%d", (int)file_stat.st_size);

        /* Sending file size */
        //len = send(peer_socket, file_size, sizeof(file_size), 0);
        len = send(peer_socket, &file_stat.st_size, sizeof(file_stat.st_size), 0);
        if (len < 0)
        {
              fprintf(stderr, "Error on sending greetings --> %s\n", strerror(errno));

              exit(EXIT_FAILURE);
        }

        //fprintf(stdout, "Server sent %d bytes for the size\n", (int)len);

        char *buffer = new char[file_stat.st_size];

        offset = 0;
        remain_data = file_stat.st_size;
        /* Sending file data */
        struct timeval stop, start;
        gettimeofday(&start, NULL);
        while (((sent_bytes = sendfile(peer_socket, fd,NULL, buffsize)) > 0) && (remain_data > 0))
        {
           //     fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
                remain_data -= sent_bytes;
         //       fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
        }
        gettimeofday(&stop, NULL);

	double elapsedTime = (stop.tv_sec - start.tv_sec); //sec
	elapsedTime += (stop.tv_usec - start.tv_usec) / 1000000.0; //usec to sec

        double mib = (double)file_stat.st_size/ 1000000.0/ elapsedTime ;
        double mb = (double)file_stat.st_size/ 1048576.0/ elapsedTime ;

        //fprintf(stdout, "RECEIVED:%ld:TIME:%ld:REMAIN:%ld:MBReceived:%.3lf:MB/s:%.3lf\n", (long)dataRetrieved, stop.tv_usec - start.tv_usec, (long)remain_data, dataRetrieved/(double)1048576 , mb);

        fprintf(stdout, "SEND:%ld:TIME:%lf:BReceived:%ld:MiB/s:%.3lf\n:MB/s:%.3lf\n", (long)file_stat.st_size,elapsedTime,(long) file_stat.st_size , mib,mb);

//        printf("off:%d\n",offset);

  //      printf("%d\n",sent_bytes);
        close(peer_socket);
        close(server_socket);

        return 0;
}
