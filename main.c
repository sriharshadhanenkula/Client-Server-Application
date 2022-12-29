#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#include "encDec.h"

char inputString[100000];
char binaryData[100000];
char dataReceivedFromClient[100000];
int choice;

int main(int argc, char *argv[])
{
    printf("Enter 1 for CRC and 2 for Hamming \n");
    scanf("%d", &choice); //  choice of the user
    int pid1 = fork();
    if (pid1 == 0)
    {

        int id2 = fork(); //  child process
        if (id2 == 0)
        {
            readInputFromFile(); //  read the input from the file

            printf("My Input %s", inputString);             //  print the input string
            execl("dataLink", "0", inputString, "2", NULL); //  call the dataLink layer
            perror("execl");
        }
        else
        {
            wait(NULL);
            printf("child process \n"); //  child process
            readBinaryDataFromFile();   //  read the binary data from the file
            printf("Choice: %d \n", choice);

            execl("client", binaryData, NULL); //  call the client
        }
    }
    else
    {
        wait(NULL);
        FILE *fp3;
        fp3 = fopen("client.txt", "r");
        int i = 0;
        char c;
        while ((c = fgetc(fp3)) != EOF)
        {
            dataReceivedFromClient[i] = c;
            i++;
        }
        dataReceivedFromClient[i] = '\0';

        fclose(fp3);
        // printf("Data received from client: %s \n", dataReceivedFromClient);
        execl("physical", "1", dataReceivedFromClient, NULL); //  call the physical layer
    }

    return 0;
}

void readDataReceivedFromClient()
{
    FILE *fp;
    fp = fopen("binaryDataReceivedByClient", "r"); //  open the file to read the data received from the client
    char c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        dataReceivedFromClient[i] = c;
        i++;
    }
    dataReceivedFromClient[i] = '\0'; //  null character
    fclose(fp);
}

void readBinaryDataFromFile()
{
    FILE *fp;
    fp = fopen("filename.binf", "r"); //  open the file in read mode

    char c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        binaryData[i] = c;
        i++;
    }
    binaryData[i] = '\0';
    fclose(fp);
}

void readInputFromFile()
{
    FILE *fp;
    fp = fopen("intext.txt", "r"); //  read the input from the file

    char c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        inputString[i] = c;
        i++;
    }
    inputString[i] = '\0';
    fclose(fp);
}