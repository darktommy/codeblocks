#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED


//tones definitios (indexes in tones[])
#define PAUSE 0
#define B7 126
#define A7d 133
#define B7b 133
#define A7 141
#define G7d 149
#define A7b 149
#define G7 158
#define F7d 168
#define G7b 168
#define F7 178
#define E7 189
#define D7d 200
#define E7b 200
#define D7 212
#define C7d 224
#define D7b 224
#define C7 238
#define B6 252
#define A6d 267
#define B6b 267
#define A6 283
#define G6d 300
#define A6b 300
#define G6 318
#define F6d 337
#define G6b 337
#define F6 357
#define E6 378
#define D6d 401
#define E6b 401
#define D6 425
#define C6d 450
#define D6b 450
#define C6 477
#define B5 505
#define A5d 535
#define B5b 535
#define A5 567
#define G5d 601
#define A5b 601
#define G5 637
#define F5d 675
#define G5b 675
#define F5 715
#define E5 757
#define D5d 803
#define E5b 803
#define D5 850
#define C5d 901
#define D5b 901
#define C5 955
#define B4 1011
#define A4d 1072
#define B4b 1072
#define A4 1135
#define G4d 1203
#define A4b 1203
#define G4 1275
#define F4d 1350
#define G4b 1350
#define F4 1431
#define E4 1516
#define D4d 1606
#define E4b 1606
#define D4 1702
#define C4d 1803
#define D4b 1803
#define C4 1910
#define B3 2024
#define A3d 2144
#define B3b 2144
#define A3 2272
#define G3d 2407
#define A3b 2407
#define G3 2550
#define F3d 2702
#define G3b 2702
#define F3 2862
#define E3 3033
#define D3d 3213
#define E3b 3213
#define D3 3404
#define C3d 3607
#define D3b 3607
#define C3 3821
#define B2 4049
#define A2d 4289
#define B2b 4289
#define A2 4544
#define G2d 4815
#define A2b 4815
#define G2 5101
#define F2d 5404
#define G2b 5404
#define F2 5726
#define E2 6066
#define D2d 6427
#define E2b 6427
#define D2 6809
#define C2d 7214
#define D2b 7214
#define C2 7644
#define B1 8098
#define A1d 8580
#define B1b 8580
#define A1 9090
#define G1d 9630
#define A1b 9630
#define G1 10203
#define F1d 10810
#define G1b 10810
#define F1 11453
#define E1 12134
#define D1d 12855
#define E1b 12855
#define D1 13620
#define C1d 14430
#define D1b 14430
#define C1 15288
#define B0 16197
#define A0d 17160
#define B0b 17160
#define A0 18181

#define END 65535
/*
uint16_t tones[] =
{
4544,//A2
4049,
3821,
3607,
3404,
3033,
2862,
2702,
2407,
2272,//A3
2144,
2024,
1803,
1702,
1606,
1516,
1431,
1350,
1203,
1135,//A4
1072,
1011,
955,
901,
850,
757,
675,
};
*/

//definitions of durations (indexes in durations[])
#define d2 0
#define d4 1
#define d8 2
#define d16 3
#define d32 4

uint16_t durations[]=
{
2500,
1250,
625,
313,
156
};

uint16_t melody[]=
{
E5,d16,
D5,d16,
F4d,d8,
G4d,d8,
C5d,d16,
B4,d16,
D4,d8,
E4,d8,
B4,d16,
A4,d16,
C4d,d8,
E4,d8,
A4,d4,
PAUSE,d2,
PAUSE,d2,
END
};


//first - tone
//second - duration


#endif // AUDIO_H_INCLUDED
