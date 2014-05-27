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

int fd; /* Файловый дескриптор для порта */
char buf[512];/*размер зависит от размера строки принимаемых данных*/
int outa=0;
int cntIn;

typedef struct _pixel
{
    char r;
    char g;
    char b;
} pixel;

int imgWidth = 0,imgHeight = 0;
pixel *imgData = 0;

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
/*
void echoImg()
{
    int i,j;
    char* ptr = header_data;
    for (i=0;i<height;i++)
    {
        for (j=0;j<width;j++)
        {
            if(*ptr == 0)
                printf("#");
            else
                printf("_");

            ptr++;
        }
        printf("\n");
    }
}
*/

void sendCommand(char* cmd)
{
    int cnt;
    write(fd,cmd, strlen(cmd));
    char bufchar[255];
    cnt = read(fd,bufchar,255);
    bufchar[cnt] = 0;
    if(!strstr(bufchar,"ok"))
        perror("error");

    return;
}
/*
void print_image()
{
    int i,j;
    char* ptr = header_data;

    sendCommand("x0\r");
    usleep(200000);
    sendCommand("x+10\r");
    usleep(50000);

    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            if(*ptr == 0)
            {
                sendCommand("z0\r");
                usleep(100000);
                sendCommand("z+20\r");
                usleep(150000);
            }
            sendCommand("x+5\r");
            usleep(100000);
            ptr++;
        }
        sendCommand("x0\r");
        usleep(500000);
        sendCommand("x+10\r");
        usleep(50000);

        sendCommand("y+5\r");
        usleep(100000);
        printf("i=%i\n",i);
    }

    sendCommand("x0\r");
    sendCommand("x+150\r");
    sendCommand("y0\r");
    sendCommand("y+150\r");;
    sendCommand("z+50\r");
}
*/
int main()
{
    readImage("/home/max/img.ppm");
    if(imgData != NULL)
        free(imgData);
    return 0;

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY); /*'open_port()' - Открывает последовательный порт */
    if (fd == -1)
    {
      /*
       * Возвращает файловый дескриптор при успехе или -1 при ошибке.
       */
       printf("error port\n");
       perror("open_port: Unable to open /dev/ttyUSBn - ");
       return 1;
    }
    else
    {
     struct termios options; /*структура для установки порта*/
     tcgetattr(fd, &options); /*читает пораметры порта*/

     cfsetispeed(&options, B9600); /*установка скорости порта*/
     cfsetospeed(&options, B9600); /*установка скорости порта*/

     options.c_cflag &= ~PARENB; /*выкл проверка четности*/
     options.c_cflag &= ~CSTOPB; /*выкл 2-х стобит, вкл 1 стопбит*/
     options.c_cflag &= ~CSIZE; /*выкл битовой маски*/
     options.c_cflag |= CS8; /*вкл 8бит*/
     options.c_cflag |= CREAD;
     //options.c_iflag = IGNPAR | ICRNL;
     options.c_cc[VTIME]    = 1;   /* inter-character timer unused */
     options.c_cc[VMIN]     = 3;   /* blocking read until 5 chars received */
     tcsetattr(fd, TCSANOW, &options); /*сохронения параметров порта*/

    }
    sleep(1);
    //print_image();
    close(fd);
    return 0;
}
