#ifndef GPS_H_INCLUDED
#define GPS_H_INCLUDED

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

typedef struct _data
{
    //double latitude;
    //double longtitude;
    //double speed;

    uint8_t valid;

    char strLatitude[13];
    char strLongtitude[13];
    char strSpeed[8];
    char NS;
    char WE;
}data;

data position;

unsigned char hex2bin( char c );
uint8_t check_crc( char *buf );
uint8_t get_crc( char * buf);
char* next_field(char* buf);
uint8_t parseGPGLL(char* buf, data* d);
uint8_t parseGPRMC(char* buf, data* d);

#endif // GPS_H_INCLUDED
