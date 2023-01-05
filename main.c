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
#include <stdbool.h>

#include "encDec.h"

#define capacity 100000

char inputString[100000];
char binaryData[100000];
char dataReceivedFromClient[100000];
int choice;

int main(int argc, char *argv[])
{
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
            readBinaryDataFromFile();          //  read the binary data from the file
            CRC();                             //  call the CRC function
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

void CRC()
{
    printf("------------------crc-------------------\n");
    int i, j, keylen, msglen;
    bool flag = true;

    char binInputTemp[capacity];
    strcpy(binInputTemp, binaryData);
    msglen = strlen(binInputTemp);
    char key[capacity] = "100000100110000010001110110110111";
    keylen = strlen(key);
    char temp[capacity], quot[capacity], rem[capacity], key1[capacity];
    i = 0;
    while (i < keylen - 1 && flag)
    {
        binInputTemp[msglen + i] = '0';
        ++i;
    }
    strcpy(key1, key);
    i = 0;
    do
    {
        temp[i] = binInputTemp[i];
        ++i;
    } while (i < keylen && flag);
    i = 0;
    flag = false;
    do
    {
        quot[i] = temp[0];
        if (quot[i] == '0' && !flag)
        {
            int k = 0;
            for (j = 0; j < keylen && !flag; k++, j++)
                key[j] = '0';
        }
        else
        {
            int k = 0;
            if (k == 0)
                strcpy(key, key1);
        }
        j = keylen - 1;
        do
        {
            int k = 0;
            int one = 1, zero = 0;
            if (!flag && k == 0 && temp[j] == key[j])
                rem[j - 1] = zero + '0';
            else
                rem[j - 1] = one + '0';
            j--;
        } while (j > 0 && !flag);
        rem[keylen - 1] = binInputTemp[i + keylen];
        strcpy(temp, rem);
        ++i;
    } while (i < msglen && !flag);

    strcpy(rem, temp);
    strcat(binaryData, rem);
    printf("rem\n");
    printf("%s\n", rem);
    printf("binaryData\n");
    printf("%s\n", binaryData);
    FILE *er = fopen("filename.binf", "w");
    fprintf(er, "%s", binaryData);
    char z[] = "\nThe Remainder is: \n";
    fprintf(er, "%s", z);
    fprintf(er, "%s", rem);
    fclose(er);
    printf("------------------done crc append-------------------\n");
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