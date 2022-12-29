#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "encDec.h"

char inputToBinaryArray[10000];
int p = 0;
char stringToDecode[10000];
char myString[10000];
int val;
char flag[3];

int main(int argc, char *argv[])
{
    if (strcmp(argv[0], "0") == 0) // if the argument is 0
    {
        char framedString[10000];
        strcpy(framedString, argv[1]); // copying the input string from the argument
        splitFrames(framedString);     // splitting the frames
        char fileFlag2 = argv[2][0];   // copying the file flag from the argument

        getFromPhysical(fileFlag2);
    }

    else if (strcmp(argv[0], "1") == 0)
    {
        char sNew[10000];
        strcpy(sNew, argv[1]);

        sendDecodeToPhysical(sNew);                   // sending the string to the physical layer
        execl("dataLink", "1", flag, myString, NULL); // calling the dataLink program
    }

    return 0;
}

void splitFrames(char *c)
{
    if (strlen(c) > 32)
    {

        int length = strlen(c); // length of the string
        int i = 0;
        while (length / 38 > 0)
        {
            char myString[38]; // string to store the frame
            int j = 0;
            while (j < 38)
            {
                myString[j] = c[i];
                i++;
                j++;
            }
            myString[j] = '\0';
            length = length - 38; // length of the string after removing the frame
            int h = strlen(myString);
            sendToPhysical(myString, h); // sending the frames to the physical layer
        }
        char s[31];
        int j = i + length; // j is the index of the last character of the string
        int k = 0;
        while (i < j)
        {
            s[k] = c[i];
            i++;
            k++;
        }
        s[k] = '\0';

        int h = strlen(s);    // length of the last frame
        sendToPhysical(s, h); // sending the frames to the physical layer
    }
    else
    {

        int myLength = strlen(c); // length of the string
        int count = 0;
        int i = 4;
        while (i < myLength) // converting the length of the string to integer
        {
            count++;
            i++;
        }
        int h;
        if (count < 11) // if the length of string is less than 11
        {
            h = myLength - 5;
        }
        else
        {
            h = myLength - 6;
        }

        sendToPhysical(c, h); // sending the frames to the physical layer
    }
}

//---------------Encode--------------------

void toBinary(char *c) // convert string to binary
{
    int i = 0;
    while (c[i] != '\0')
    {
        int j = 7;
        while (j >= 0)
        {
            if (c[i] & (1 << j)) // if the bit is 1 then add 1 to the array
            {
                inputToBinaryArray[p] = '1'; // if the bit is 1, then store 1 in the array
                p = p + 1;
            }
            else
            {
                inputToBinaryArray[p] = '0'; // if the bit is 0, then store 0 in the array
                p = p + 1;
            }
            j--;
        }
        i++;
    }
    inputToBinaryArray[p] = '\0';
}

void decimalToBinary(int n)
{
    long bin = 0;
    long dec = n;
    long i = 1;

    while (dec > 0) // converting decimal to binary
    {
        bin += (dec % 2) * i; // storing the remainder in the array
        dec /= 2;             // dividing the number by 2
        i *= 10;
    }
    char binary[10];
    int k = 0;
    for (int j = 0; j < 8; j++)
    {
        binary[k] = (bin % 10) + '0'; // converting the binary to string
        bin = bin / 10;
        k++;
    }
    for (int j = 7; j >= 0; j--)
    {
        inputToBinaryArray[p] = binary[j]; // storing the binary in the array
        p++;
    }
}

void toBinaryFirst()
{
    decimalToBinary(22); // adding the start flag
    decimalToBinary(22);
}

void convertCharToBinary(char *c, int n)
{
    toBinaryFirst(); // adding the start and end flags

    int length = n;
    decimalToBinary(length); // converting the length of the string to binary

    int i;
    if (n < 15)
    {
        i = 5; // if the length of the string is less than 10
    }
    else
    {
        i = 6; // if the length of the string is greater than 10
    }
    char userInput[2000];
    int k = 0;
    while (c[i] != '\0')
    {
        userInput[k] = c[i]; // copying the string to the array
        k++;
        i++;
    }
    userInput[k] = '\0';
    toBinary(userInput); // converting the string to binary
}

void addOddParityBits()
{

    int i = 0;
    int j = 0;
    int k = 0;
    int count = 0;
    while (inputToBinaryArray[i] != '\0')
    {
        if (i != 0 && i % 8 == 0)
        {
            count = 0;
            j = k; // j is the index of the last bit of the byte
            while (j < i)
            {
                if (inputToBinaryArray[j] == '1')
                {
                    count++; // counting the number of 1's
                }
                j++;
            }
            if (count % 2 == 0)
            {
                inputToBinaryArray[k] = '1'; // adding 1 to the array
            }
            else
            {
                inputToBinaryArray[k] = '0'; // adding odd parity bits
            }
            k = k + 8;
        }
        i++;
    }
    int n = strlen(inputToBinaryArray);
    int count1 = 0;
    for (int i = n - 8; i < n; i++)
    { // adding the parity bit for the last frame
        if (inputToBinaryArray[i] == '1')
        {
            count1++; // count the number of 1's in the last byte
        }
    }
    if (count1 % 2 == 0)
    {
        inputToBinaryArray[n - 8] = '1'; // adding the last parity bit
    }
    else
    {
        inputToBinaryArray[n - 8] = '0'; // adding the last parity bit
    }
}

void storeDataInFile() // store data in file
{
    FILE *fptr;
    fptr = fopen("filename.binf", "w");
    if (fptr == NULL)
    {
        printf("Error!"); // if file is not created
    }
    fprintf(fptr, "%s", inputToBinaryArray); // writing the binary string to the file
    fclose(fptr);
}

void storeDataInChckFile()
{
    FILE *fp4;
    fp4 = fopen("filename.chck", "w");      // store data in file
    fprintf(fp4, "%s", inputToBinaryArray); // store data in file
    fclose(fp4);
}

void getFromPhysical(char fileFlag)
{
    addOddParityBits(); // add the odd parity bits

    if (fileFlag == '2')
    {
        storeDataInFile(); // store the data in the file
    }
    else if (fileFlag == '3')
    {
        storeDataInChckFile();
    }
}

void sendToPhysical(char synChar[], int n)
{

    convertCharToBinary(synChar, n);
}

// -------------Decode-----------------

void removeOddParityBits(char *c)
{
    int i = 0; // i is the index of the array
    while (c[i] != '\0')
    {
        if (i % 8 == 0)
        {
            if (c[i] == '1')
            {
                c[i] = '0'; // if the bit is 1, then change it to 0
            }
        }
        stringToDecode[i] = c[i];
        i++;
    }
}

int myPower(int n, int m)
{
    int result = 1;
    for (int i = 0; i < m; i++)
    {
        result = result * n; // calculating the power of the number
    }
    return result;
}

void convertToString()
{

    int i = 0;
    int m = 0;
    int count = 0;
    while (stringToDecode[i] != '\0')
    {
        int j = 7;
        int sum = 0;

        while (j >= 0)
        {
            if (stringToDecode[i] == '1')
            {
                sum = sum + myPower(2, j); // converting binary to decimal
            }
            j--;
            i++;
        }
        count++;
        if (count == 36)
        {
            count = 1; // count is 1 because we have to skip the first 36 bits
        }

        if (count < 4)
        {
            char str[2];
            sprintf(str, "%d", sum); // converting integer to string

            if (count == 3)
            {
                val = strlen(str);
            }

            int z = 0;
            while (str[z] != '\0') // convert the integer to string
            {
                myString[m] = str[z];
                m++;
                z++;
            }
        }
        else
        {
            myString[m] = sum; // store the character in the string
            m++;
        }
    }
    myString[m] = '\0';
}

void sendDecodeToPhysical(char sNew[])
{

    removeOddParityBits(sNew); // remove the odd parity bits

    convertToString(); // convert the binary to string

    sprintf(flag, "%d", val);
}
