#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>

sem_t bufferEmpty1, bufferFull1, bufferEmpty2, bufferFull2, bufferEmpty3, bufferFull3, bufferEmpty4;
sem_t bufferFull4, bufferEmpty5, bufferFull5, bufferEmpty6, bufferFull6;

pthread_mutex_t mutex1, mutex2, mutex3, mutex4, mutex5, mutex6;

char queue1[10], queue2[10], queue3[10], queue4[10], queue5[10], queue6[10];
int count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0, count6 = 0;

int p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0, p7 = 0, p8 = 0, p9 = 0, p10 = 0, p11 = 0, p12 = 0;

int totalSum = 0;

char finalString[1000000];
int finalCount = 0;


void *charAFunction(void *arg)     
{   
    char *charA = (char *)arg;
    int i = 0;
    for (i = 0; i < strlen(charA); i++)
    {
        // printf("   A.................\n");
        sem_wait(&bufferEmpty1);            // wait for buffer to be empty
        pthread_mutex_lock(&mutex1);            // lock the mutex
        if (p1 == 10)
        {
            p1 = 0;
        }
        if (charA[i] == 'a')
        {
            queue1[p1] = 'A';
        }
        else
        {
            queue1[p1] = charA[i];
        }
        p1++;
        count1++;
        pthread_mutex_unlock(&mutex1);          // unlock the mutex
        sem_post(&bufferFull1);        // signal that buffer is full
    }
    return NULL;
}

void *charEFunction(void *arg)
{
    int *len = (int *)arg;
    int i = 0;
    int j = 0;
    char x;
    while (i < *len)
    {
        // printf("  E.................\n");
        sem_wait(&bufferFull1);             // wait for buffer to be full
        pthread_mutex_lock(&mutex1);        // lock the mutex
        if (p2 == 10)
        {
            p2 = 0;
        }
        x = queue1[p2];
        p2++;
        count1--;
        pthread_mutex_unlock(&mutex1);          // unlock
        sem_post(&bufferEmpty1);            // signal
        i++;

        sem_wait(&bufferEmpty2);                // wait
        pthread_mutex_lock(&mutex2);            // lock the mutex
        if (p3 == 10)   
        {
            p3 = 0;
        }
        if (x == 'e')                                           
        {
            queue2[p3] = 'E';               
        }
        else
        {
            queue2[p3] = x;
        }
        p3++;
        count2++;
        pthread_mutex_unlock(&mutex2);      // unlock the mutex
        sem_post(&bufferFull2);
    }

    return NULL;
}

void *charIFunction(void *arg)
{
    int *len = (int *)arg;          // length of the string
    int i = 0;
    char x;
    while (i < *len)
    {
        // printf("   I................ \n");
        sem_wait(&bufferFull2);
        pthread_mutex_lock(&mutex2);        // lock the mutex
        if (p4 == 10)
        {
            p4 = 0;
        }
        x = queue2[p4];             // get the character from the queue
        p4++;
        count2--;
        pthread_mutex_unlock(&mutex2);      // unlock the mutex
        sem_post(&bufferEmpty2);            // signal
        // printf("%c", x);
        i++;
        sem_wait(&bufferEmpty3);
        pthread_mutex_lock(&mutex3);            // lock
        if (p5 == 10)
        {
            p5 = 0;
        }
        if (x == 'i')
        {
            queue3[p5] = 'I';               // put the character in the queue
        }
        else
        {
            queue3[p5] = x;
        }
        p5++;
        count3++;
        pthread_mutex_unlock(&mutex3);          // unlock
        sem_post(&bufferFull3);         // signal
    }
}

void *charOFunction(void *arg)
{
    int *len = (int *)arg;
    int i = 0;
    char x;
    while (i < *len)
    {
        // printf("   O.............. \n");
        sem_wait(&bufferFull3);             // wait
        pthread_mutex_lock(&mutex3);        // lock the mutex
        if (p6 == 10)
        {
            p6 = 0;
        }
        x = queue3[p6];
        p6++;
        count3--;
        pthread_mutex_unlock(&mutex3);          // unlock
        sem_post(&bufferEmpty3);                // signal
        // printf("%c", x);
        i++;
        sem_wait(&bufferEmpty4);
        pthread_mutex_lock(&mutex4);
        if (p7 == 10)
        {
            p7 = 0;
        }
        if (x == 'o')
        {
            queue4[p7] = 'O';           // put the character in the queue
        }
        else
        {
            queue4[p7] = x;
        }
        p7++;
        count4++;
        pthread_mutex_unlock(&mutex4);      // unlock the mutex
        sem_post(&bufferFull4);     // signal that buffer is full
    }
}

void *charUFunction(void *arg)
{
    int *len = (int *)arg;      // length of the string
    int i = 0;
    char x;
    while (i < *len)
    {
        // printf("  U......... \n");
        sem_wait(&bufferFull4);
        pthread_mutex_lock(&mutex4);
        if (p8 == 10)
        {
            p8 = 0;
        }
        x = queue4[p8];
        p8++;
        count4--;
        pthread_mutex_unlock(&mutex4);      // unlock mutex
        sem_post(&bufferEmpty4);
        // printf("%c", x);
        i++;
        sem_wait(&bufferEmpty5);            // wait for buffer to be empty
        pthread_mutex_lock(&mutex5);        // lock mutex
        if (p9 == 10)
        {
            p9 = 0;
        }
        if (x == 'u')
        {
            queue5[p9] = 'U';
        }
        else
        {
            queue5[p9] = x;
        }
        p9++;
        count5++;
        pthread_mutex_unlock(&mutex5);
        sem_post(&bufferFull5);
    }
}

void *digitFunction(void *arg)
{
    int *len = (int *)arg;
    int i = 0;
    char x;
    while (i < *len)
    {
        // printf("   digit........... \n");
        sem_wait(&bufferFull5);             // wait for buffer to be full
        pthread_mutex_lock(&mutex5);        // lock
        if (p10 == 10)
        {
            p10 = 0;
        }
        x = queue5[p10];
        p10++;
        count5--;
        pthread_mutex_unlock(&mutex5);     
        sem_post(&bufferEmpty5);
        // printf("%c", x);
        i++;
        sem_wait(&bufferEmpty6);            // bufferEmpty6
        pthread_mutex_lock(&mutex6);        // mutex6
        if (p11 == 10)
        {
            p11 = 0;
        }
        if (x >= '0' && x <= '9')
        {
            queue6[p11] = x;
            totalSum += x - '0';
        }
        else
        {
            queue6[p11] = x;
        }

        p11++;
        count6++;
        pthread_mutex_unlock(&mutex6);      //unlock mutex
        sem_post(&bufferFull6);         //increment bufferFull
    }
}

void *writerFunction(void *arg)     
{
    int *len = (int *)arg;          // get the length of the string
    int i = 0;
    char x;
    while (i < *len)
    {
        // printf("  writer..... \n");
        sem_wait(&bufferFull6);             // wait for the buffer to be full
        pthread_mutex_lock(&mutex6);        // lock the mutex
        if (p12 == 10)
        {
            p12 = 0;
        }
        x = queue6[p12];
        p12++;
        count6--;
        pthread_mutex_unlock(&mutex6);      // unlock the mutex
        sem_post(&bufferEmpty6);        // post to the buffer
        // printf("%c", x);
        finalString[finalCount] = x;
        finalCount++;
        i++;
    }
    finalString[finalCount] = '\0';
    char chrToInt[1000];
    sprintf(chrToInt, "%d", totalSum);
    // printf("Total Sum in String: %s \n", chrToInt);
    char additionalString[] = " Sum of digits is: ";
    strcat(additionalString, chrToInt);         // Concatenate two strings
    strcat(finalString, additionalString);
    // printf("Final String: %s \n", finalString);
}

int main(int argc, char *argv[])
{

    int pid2 = fork();
    if (pid2 == 0)
    {
        char myBinaryString[100000];

        strcpy(myBinaryString, argv[0]);
        execl("physical", "1", myBinaryString, NULL);           // 1 is the argument for physical memory
        perror("execl");
    }
    else
    {
        wait(NULL);
        FILE *fp;
        fp = fopen("result.txt", "r");
        int m = 0;
        char c;
        char s[100000];
        while ((c = fgetc(fp)) != EOF)
        {
            s[m] = c;
            m++;
        }
        s[m] = '\0';
        pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7;        //thread id for each thread
        pthread_mutex_init(&mutex1, NULL);                                       //mutex initialization
        pthread_mutex_init(&mutex2, NULL);
        pthread_mutex_init(&mutex3, NULL);
        pthread_mutex_init(&mutex4, NULL);
        pthread_mutex_init(&mutex5, NULL);
        pthread_mutex_init(&mutex6, NULL);

        sem_init(&bufferEmpty1, 0, 10);                                 //semaphore initialization
        sem_init(&bufferFull1, 0, 0);
        sem_init(&bufferEmpty2, 0, 10);
        sem_init(&bufferFull2, 0, 0);
        sem_init(&bufferEmpty3, 0, 10);
        sem_init(&bufferFull3, 0, 0);
        sem_init(&bufferEmpty4, 0, 10);
        sem_init(&bufferFull4, 0, 0);
        sem_init(&bufferEmpty5, 0, 10);
        sem_init(&bufferFull5, 0, 0);
        sem_init(&bufferEmpty6, 0, 10);
        sem_init(&bufferFull6, 0, 0);

        char *str = s;
        int l = strlen(str);
        int *len = &l;

        pthread_create(&thread1, NULL, charAFunction, (void *)str);     // A to E thread creation and passing string as argument
        pthread_create(&thread2, NULL, charEFunction, (void *)len);
        pthread_create(&thread3, NULL, charIFunction, (void *)len);
        pthread_create(&thread4, NULL, charOFunction, (void *)len);
        pthread_create(&thread5, NULL, charUFunction, (void *)len);
        pthread_create(&thread6, NULL, digitFunction, (void *)len);
        pthread_create(&thread7, NULL, writerFunction, (void *)len);

        pthread_join(thread1, NULL);        // A to E thread joining with main thread 
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);
        pthread_join(thread6, NULL);
        pthread_join(thread7, NULL);

        sem_destroy(&bufferEmpty1);     // A to E semaphore destroying 
        sem_destroy(&bufferFull1);
        sem_destroy(&bufferEmpty2);
        sem_destroy(&bufferFull2);
        sem_destroy(&bufferEmpty3);
        sem_destroy(&bufferFull3);
        sem_destroy(&bufferEmpty4);
        sem_destroy(&bufferFull4);
        sem_destroy(&bufferEmpty5);
        sem_destroy(&bufferFull5);
        sem_destroy(&bufferEmpty6);
        sem_destroy(&bufferFull6);

        pthread_mutex_destroy(&mutex1);     // A to E mutex destroying
        pthread_mutex_destroy(&mutex2);
        pthread_mutex_destroy(&mutex3);
        pthread_mutex_destroy(&mutex4);
        pthread_mutex_destroy(&mutex5);
        pthread_mutex_destroy(&mutex6);
        printf("Final String: %s \n", finalString);
        execl("dataLink", "0", finalString, "2", NULL);         // passing final string to dataLink layer
    }

    return 0;
}