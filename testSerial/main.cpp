#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"
using namespace std;

int main()
{
    int portNum = 16;
    cout << "Hello world!" << endl;

    if(RS232_OpenComport(portNum,9600))
    {
        cout << "Cannot open serial port :(" << endl;
        return 0;
    }

    int angle,gas;
    int n;
    unsigned char receive[256];
    unsigned char buf[6];
    while(1)
    {
        scanf("%i %i", &angle, &gas);

        unsigned short int checksum;
        checksum = ((unsigned short int)angle) ^ ((unsigned short int) gas);
        //printf("checksum: 0x%X",checksum);

        buf[0] = (unsigned char)angle;
        memcpy(buf+1,(void*)&gas, 2);
        memcpy(buf+3,(void*)&checksum,2);
        buf[5] = 0x4F;

        RS232_SendBuf(portNum,buf,6);

        usleep(100000);


        if((n = RS232_PollComport(portNum,receive,255))>0)
        {
            receive[n]=0;
            printf("%s\n",receive);
        }

    }
    RS232_CloseComport(portNum);
    return 0;
}
