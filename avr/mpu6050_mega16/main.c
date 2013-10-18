/*
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "serial.h"
#include "mpu6050.h"

int main(void)
{

    serialInit(SPEED9600);
    sei();
    serialWrite("Hello\n");
    mpu6050_init();

    _delay_ms(100);

    if(mpu6050_testConnection())
        serialWrite("Connection OK\n");
    else
        serialWrite("Connection FAIL\n");

    double roll,pitch,yaw;
    //double acc[3],gyro[3];
    while(1)
    {

        mpu6050_getRollPitchYaw(&pitch,&roll,&yaw);

        serialWrite("Angles: [");
        serialPrintDouble(pitch);
        serialWrite(", ");
        serialPrintDouble(roll);
        serialWrite(", ");
        serialPrintDouble(yaw);
        serialWrite("] \n");
/*
        mpu6050_getConvData(&acc[0],&acc[1],&acc[2],&gyro[0],&gyro[1],&gyro[2]);
        serialWrite("ACC: ");
        serialPrintFloat(acc[0]);
        serialWrite(" ");
        serialPrintFloat(acc[1]);
        serialWrite(" ");
        serialPrintFloat(acc[2]);
        serialWrite("\n");*/
        _delay_ms(100);
    }
    serialEnd();


    return 0;
}
