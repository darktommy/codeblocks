#include <string.h>
#include "nmea.h"

nmea_message_t nmea_messages[] =
{
  {
    .id = "GPRMC",
    .parser = nmea_rmc_parser,
    .data = &nmea_rmc_data,
  },
};


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


char nmea_check_crc( char *buf )
{
  unsigned char crc = 0;
  while( *buf != '*' ) {
     crc ^= *buf++;
  }

  return ( ( crc & 0x0F ) == hex2bin( *buf ) ) &&
         ( ( crc >> 4   ) == hex2bin( *(buf+1) ) );
}



char *nmea_next_field( char* buf)
{
    // return strtok( buf, ',' );
    while( *buf++ != ',' );
      return buf;
}




//$GPRMC,175110.00,A,6854.45259,N,03305.02761,E,0.025,,210214,,,A*77
char nmea_rmc_parser( char *buf, void *data )
{
  nmea_rmc_t* rmc = (nmea_rmc_t*)data;
  memset( rmc, 0, sizeof( nmea_rmc_t ) );

  rmc->utc_time = atof( buf );
  buf = nmea_next_field( buf );

  if( *buf == 'A') {
    rmc->valid = 1;
  }

  buf = nmea_next_field( buf );
  rmc->latitude = atof( buf );
  buf = nmea_next_field( buf );
  if( *buf == 'S') {
    rmc->latitude = -rmc->latitude;
  }

  buf = nmea_next_field( buf );
  rmc->longitude = atof( buf );

  buf = nmea_next_field( buf );
  if( *buf == 'W') {
    rmc->longitude = -rmc->longitude;
  }

  buf = nmea_next_field( buf );
  rmc->speed  = atof( buf );

  buf = nmea_next_field( buf );
  rmc->direction  = atof( buf );

  buf = nmea_next_field( buf );
  rmc->data  = atol( buf );

  return 0;
}



unsigned char nmea_parser( char* buf )
{
  unsigned char i;

  if( *buf++ != '$' ) {
    return -1;
  }

  if( !nmea_check_crc( buf )  ) {
    return -1; //CRC fail
  }

  for( i = 0; i< ARRAY_SIZE( nmea_messages ); i++ ) {
    if( strncmp( buf, nmea_messages[i].id, strlen( nmea_messages[i].id ) ) == 0 ) {
      if( nmea_messages[i].parser ) {
        buf = nmea_next_field( buf );
        return nmea_messages[i].parser( buf, nmea_messages[i].data );
      }
      return -1;
    }
  }

  return -1;
}
