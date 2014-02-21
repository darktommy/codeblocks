#ifndef NMEA__H
#define NMEA__H

#define ARRAY_SIZE( array )  (sizeof(array)/sizeof(array[0]))



unsigned char hex2bin( char c );
char nmea_check_crc( char *buf );
char *nmea_next_field( char* buf);
char nmea_rmc_parser( char *buf, void *data );
unsigned char nmea_parser( char* buf );

//структура, описывающая nmea сообщение
typedef struct _nmea_message {
  char * id; //идентификатор сообщения
  char (*parser)( char *buf, void *data ); //парсер сообщения
  void *data; //результат парсинга сообщения
} nmea_message_t;


typedef struct gms_rmc {
  double utc_time;     //время в 0-ой зоне
  unsigned char valid; //признак достоверности данных
  double latitude;     //широта
  double longitude;    //долгота
  double speed;        //скорость в милях
  double direction;    //направление движения
  unsigned long data;  //дата
} nmea_rmc_t;

nmea_rmc_t nmea_rmc_data;




#endif
