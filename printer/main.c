#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <stdlib.h>
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "rs232.h"

typedef struct _pixel
{
    char r;
    char g;
    char b;
} pixel;

int imgWidth = 0,imgHeight = 0;
pixel *imgData = 0;
int portNum = 16; //ttyUSB0

int readImage(char* imgFilename)
{
    FILE *fp = fopen(imgFilename,"r");
    if( !fp)
    {
        printf("Error opening file! Exit..\n");
        return -1;
    }

    char buf[255];
    fgets(buf,255,fp);
    if(!strstr(buf,"P6"))
    {
        printf("Wrong file! Exit...\n");
        return -1;
    }
    printf(buf);

    fgets(buf,255,fp); //metadata
    printf(buf);

    //fgets(buf,255,fp); //width height
    fscanf(fp,"%d %d\n",&imgWidth,&imgHeight);
    printf("Image Size: %dx%d\n",imgWidth, imgHeight);

    fgets(buf,255,fp); //color-depth per channel
    printf(buf);
    imgData = malloc(imgWidth*imgHeight*sizeof(pixel));

    int i,j;
    j=0;
    for (i = 0;i<imgWidth*imgHeight;i++)
    {
        imgData[i].r = fgetc(fp);
        imgData[i].g = fgetc(fp);
        imgData[i].b = fgetc(fp);
    }

    fclose(fp);
    for (i=0;i<imgHeight;i++)
    {
        for(j=0;j<imgWidth;j++)
        {
            if( imgData[i*imgWidth+j].r == 0 )
                printf("#");
            else
                printf(" ");
        }
        printf("\n");
    }

    return 0;
}


void sendCmd(char* strCmd)
{
    RS232_SendBuf(portNum,(unsigned char*)strCmd,strlen(strCmd));
    usleep(50000);

    char buf[255];
    char mainbuf[255];
    int n;
    memset(buf,0,255);
    memset(mainbuf,0,255);

    while(1)
    {
        n = RS232_PollComport(portNum,(unsigned char*)buf,255);
        if(n>0)
        {
            buf[n] = '\0';
            strcat(mainbuf,buf);
            if(strchr(buf,'\n'))
            {
                if(strstr(mainbuf,"ok"))
                {
                    memset(mainbuf,0,255);
                    break;
                }
            }
        }
    }


    //printf("OK\n");
}

void printImage()
{
    int i,j;

    sendCmd("x0\r");
    sendCmd("y0\r");

    sendCmd("x+10\r");
    sendCmd("y+10\r");
    if(!imgData)
    {
        printf("No data load. Exit..\n");
        return;
    }

    for (i=0;i<imgHeight;i++)
    {
        for(j=0;j<imgWidth;j++)
        {
            if( imgData[i*imgWidth+j].r == 0 )
            {
                sendCmd("z0\r");
                usleep(100000);
                sendCmd("z+20\r");
                usleep(100000);
            }
            sendCmd("x+5\r");
        }
        sendCmd("x0\r");
        sendCmd("x+10\r");
        sendCmd("y+5\r");
        printf("i=%i\n",i);
    }

    sendCmd("x0\r");
    sendCmd("y0\r");
}



int main()
{
    readImage("/home/max/img3.ppm");


    if(RS232_OpenComport(portNum,9600))
    {
        printf("Cannot open serial port :(\n");
        return 0;
    }

    printImage();

    if(imgData != NULL)
        free(imgData);
    return 0;
}
