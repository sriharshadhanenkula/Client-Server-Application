#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])        //  ./client 
{
    char myBinaryString[100000];
    strcpy(myBinaryString, argv[0]);
    // printf("In client \n");
    // printf("%s \n", myBinaryString);

    char *ip = "127.0.0.1";     //  localhost
    int port = 5566;            //  port number

    int sock;
    struct sockaddr_in addr;    //  socket address structure
    socklen_t addr_size;        //  socket address size
    char buffer[100000];        //  buffer to store data
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));      //  clear the address structure
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);       //  convert ip address to binary

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to the server.\n");

    bzero(buffer, 100000);
    strcpy(buffer, myBinaryString);     //  copy the binary string to buffer
    // printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);

    bzero(buffer, 100000);
    recv(sock, buffer, sizeof(buffer), 0);
    //printf("Server: %s\n", buffer);

    bzero(buffer, 100000);
    recv(sock, buffer, sizeof(buffer), 0);      //  receive the binary string from server
    //printf("Second server: %s\n", buffer);
    FILE *fp;
    fp = fopen("client.txt", "w");
    fprintf(fp, "%s", buffer);
    fclose(fp);

    close(sock);            //  close the socket
    printf("Disconnected from the server.\n");

    return 0;
}