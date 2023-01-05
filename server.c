#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wait.h>

int main()
{
    char myBinaryString[100000];

    char *ip = "127.0.0.1"; //  localhost
    int port = 5566;        //  port number

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr; //  socket address structure
    socklen_t addr_size;
    char buffer[100000];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0); //  create a socket
    if (server_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr)); //  clear the address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip); //  convert ip address to binary

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)); //  bind the socket to the address
    if (n < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port); //  print the port number

    listen(server_sock, 5);
    printf("Listening...\n");
    int p = 0;
    while (1)
    {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size); //  accept the connection
        printf("[+]Client connected.\n");

        bzero(buffer, 100000);
        recv(client_sock, buffer, sizeof(buffer), 0);
        // printf("Client: %s\n", buffer);
        strcpy(myBinaryString, buffer);

        bzero(buffer, 100000);
        strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        // printf("Server: %s\n", buffer);
        send(client_sock, buffer, strlen(buffer), 0);

        // close(client_sock);
        printf("[+]Client disconnected.\n\n");
        // printf("%s \n", myBinaryString);

        int pid4 = fork();
        if (pid4 == 0)
        {
            // execl("physical", "1", myBinaryString, NULL);
            // perror("execl");
            // printf("Server side received %s \n", myBinaryString);
            execl("receiver", myBinaryString, NULL); //  execute the receiver program
        }
        else
        {
            wait(NULL);
            FILE *fp;
            fp = fopen("filename.binf", "r"); //  open the file to read
            int har = 0;
            char DataToBeSentToClient[100000];
            char ch;
            while ((ch = fgetc(fp)) != EOF)
            {
                DataToBeSentToClient[har] = ch; //  read the data from the file
                har = har + 1;
            }
            DataToBeSentToClient[har] = '\0';
            fclose(fp);
            // printf("Data To be sent %s \n", DataToBeSentToClient);

            bzero(buffer, 100000);
            strcpy(buffer, DataToBeSentToClient);
            // printf("Server to Client in Server: %s\n", buffer);
            send(client_sock, buffer, strlen(buffer), 0); //  send the data to the client
            close(client_sock);
            break;
        }
    }

    return 0;
}