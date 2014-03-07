#include "gps.h"

unsigned char hex2bin( char c )
{
  if( ( c >= 'A') && ( c <= 'F') ) {
    return c - 'A';
  }
  else if( ( c >= 'a') && ( c <= 'f') ) {
    return c - 'a';
  }
  else {
    return c - '0';
  }
}


uint8_t check_crc( char *buf )
{
  unsigned char crc = 0;
  while( *buf != '*' ) {
     crc ^= *buf++;
  }

  return ( ( crc & 0x0F ) == hex2bin( *(buf+2) ) ) &&
         ( ( crc >> 4   ) == hex2bin( *(buf+1) ) );
}

uint8_t get_crc( char * buf)
{
    uint8_t crc = 0;
    while( *buf != '*' ) {
     crc ^= *buf++;
    }
    return crc;
}


char* next_field(char* buf)
{
    while(*buf++ != ',')
        ;

    return buf;
}


/*
uint8_t parseGPGLL(char* buf, data* d)
{
    buf = strchr(buf, '$');
    if(buf == 0) return -1;

    buf++;

    if(strncmp(buf, "GPGLL", 5) != 0) return -1;

    if(!check_crc(buf))
        return -1;


    buf = next_field(buf);
    if(*buf != ',')
        (*d).latitude = atof(buf);

    buf = next_field(buf);
    if(*buf == 'S')
        (*d).latitude = -(*d).latitude;

    buf = next_field(buf);
    if(*buf != ',')
        (*d).longtitude = atof(buf);

    buf = next_field(buf);
    if(*buf == 'W')
        (*d).longtitude = -(*d).longtitude;

    buf = next_field(buf); //skip UTC time
    buf = next_field(buf);

    if(*buf == 'A')
        (*d).valid = 1;
    else
        (*d).valid = 0;

    return 0;

}
*/
uint8_t parseGPRMC(char* buf, data* d)
{
    buf = strchr(buf, '$');
    if(buf == 0) return 0;

    buf++;

    if(strncmp(buf, "GPRMC", 5) != 0) return -1;

    if(!check_crc(buf))
        return -1;

    buf = next_field(buf); //время
    buf = next_field(buf); //валидность
    if(*buf == 'A')
        (*d).valid = 1;
    else
    {
        (*d).valid = 0;
    }


    buf = next_field(buf);   //latitude
    if(*buf != ',')
    {
        //(*d).latitude = atof(buf);
        char *p1 = next_field(buf);
        strlcpy((*d).strLatitude, buf, p1-buf);
    }

    buf = next_field(buf);  //N or S
    (*d).NS = *buf;
    //if(*buf == 'S')
    //    (*d).latitude = -(*d).latitude;

    buf = next_field(buf);  //longitude
    if(*buf != ',')
    {
        //(*d).longtitude = atof(buf);
        char *p1 = next_field(buf);
        strlcpy((*d).strLongtitude, buf, p1-buf);
    }

    buf = next_field(buf);  //W or E
    (*d).WE = *buf;
    //if(*buf == 'W')
    //(*d).longtitude = -(*d).longtitude;

    buf = next_field(buf); //скорость в узлах
    if(*buf != ',')
    {
        //(*d).speed = atof(buf);
        char *p1 = next_field(buf);
        strlcpy((*d).strSpeed, buf, p1-buf);
    }

    buf = next_field(buf); //курс (?)
    buf = next_field(buf); //дата
    //дальше какаято фигня, нам оно не надо
    return 0;

}
