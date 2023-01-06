#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "encDec.h"

char inputString[10000];
char framedString[10000];
char userString[10000];

int main(int argc, char *argv[])
{
    if (strcmp(argv[0], "0") == 0)
    {                                 // if the argument is 0, then the program is called from the child process
        strcpy(inputString, argv[1]); // copying the input string from the argument
        toAppLayer(inputString);
        char fileFlag1 = argv[2][0];
        if (fileFlag1 == '2')
        {
            execl("physical", "0", framedString, "2", NULL);
        }
        else if (fileFlag1 == '3')
        {
            execl("physical", "0", framedString, "3", NULL);
        }
    }
    else if (strcmp(argv[0], "1") == 0)
    { // if the argument is 1, then the program is called from the grandchild process
        int val = argv[1][0] - '0';
        char myString[10000];
        strcpy(myString, argv[2]);
        sendToDataLink(myString, val);
    }

    return 0;
}

void mergeFrames(char *c)
{
    strcat(framedString, c); // merging the frames
}

void sendToData(char inputString[])
{

    int len = 0;
    len = strlen(inputString); // length of the string
    if (len < 10)
    {
        char myLen[2];
        sprintf(myLen, "%d", len);               // convert the length to string
        char synChar[40] = {'2', '2', '2', '2'}; // add the synchro bits
        strcat(synChar, myLen);                  // add the length of the string to the synchro bits
        strcat(synChar, inputString);            // add the string to the synchro bits

        mergeFrames(synChar);
    }
    else
    {
        char myLen[3];
        int p = 0;
        sprintf(myLen, "%d", len);
        char synChar[40] = {'2', '2', '2', '2'};
        synChar[4] = myLen[0]; // add the first digit of the length to the synchro bits
        synChar[5] = myLen[1];
        strcat(synChar, inputString); // add the string to the synchro bits

        mergeFrames(synChar);
    }
}

void toAppLayer(char *inputString)
{ // function to send the data to the application layer
    if (strlen(inputString) > 32)
    {

        int length = strlen(inputString); // length of the string
        int i = 0;
        while (length / 32 > 0)
        { // if the length of the string is greater than 32
            char myString[32];
            int j = 0;
            while (j < 32)
            { // copy the first 32 characters to myString
                myString[j] = inputString[i];
                i++;
                j++;
            }
            myString[j] = '\0'; // add the null character to the end of the string
            length = length - 32;

            sendToData(myString);
        }
        char s[31];
        int j = i + length;
        int k = 0;

        while (i < j)
        { // copy the remaining characters to myString
            s[k] = inputString[i];
            i++;
            k++;
        }
        s[k] = '\0';
        sendToData(s);
    }
    else
    {
        sendToData(inputString); // send to data layer
    }
}

// -----------------------deFrame-------------------------------------

void sendToDataLink(char myString[], int val)
{

    int c = 0;
    int myLength = strlen(myString);
    int i = 0;

    while (myLength / 38 > 0)
    {

        i = i + 6;
        int h = i;
        while (i < h + 32) // copy the first 32 characters to myString
        {
            userString[c] = myString[i];
            i++;
            c++;
        }

        myLength = myLength - 38;
    }

    if (val == 1)
    {
        i = i + 5;
    }
    else
    {
        i = i + 6;
    }
    while (myString[i] != '\0')
    {
        userString[c] = myString[i];
        i++;
        c++;
    }

    userString[c + 1] = '\0';

    printf("%s \n", userString);
    printf("\n");

    FILE *fp;
    fp = fopen("result.txt", "w"); // storing output
    fprintf(fp, "%s", userString);

    /*FILE *fp3;
    fp3 = fopen("filename.done", "w"); // storing output in final file
    fprintf(fp3, "%s", userString);

    fclose(fp3);*/
}
