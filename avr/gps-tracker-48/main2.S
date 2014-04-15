	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.section	.text.SetTimer1Mode,"ax",@progbits
.global	SetTimer1Mode
	.type	SetTimer1Mode, @function
SetTimer1Mode:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	tst r24
	breq .L3
	cpi r24,lo8(1)
	breq .L4
	ret
.L3:
	sts 129,__zero_reg__
	sts 132+1,__zero_reg__
	sts 132,__zero_reg__
	ret
.L4:
	sts 132+1,__zero_reg__
	sts 132,__zero_reg__
	ldi r24,lo8(1)
	sts 111,r24
	sts 128,__zero_reg__
	ldi r24,lo8(4)
	sts 129,r24
	ret
	.size	SetTimer1Mode, .-SetTimer1Mode
	.section	.text.SetMode,"ax",@progbits
.global	SetMode
	.type	SetMode, @function
SetMode:
	push r28
/* prologue: function */
/* frame size = 0 */
/* stack size = 1 */
.L__stack_usage = 1
	mov r28,r24
	lds r24,currentMode
	cp r28,r24
	breq .L5
/* #APP */
 ;  68 "main.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	cpi r28,lo8(2)
	breq .L8
	cpi r28,lo8(3)
	breq .L9
	cpi r28,lo8(1)
	brne .L7
	rjmp .L10
.L9:
	cbi 0xb,2
	ldi r24,lo8(1)
	rcall SetTimer1Mode
	rjmp .L7
.L8:
	sbi 0xb,2
	ldi r24,0
	rcall SetTimer1Mode
	rjmp .L7
.L10:
	sbi 0xb,2
	ldi r24,0
	rcall SetTimer1Mode
.L7:
	sts currentMode,r28
/* #APP */
 ;  85 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L5:
/* epilogue start */
	pop r28
	ret
	.size	SetMode, .-SetMode
	.section	.text.__vector_13,"ax",@progbits
.global	__vector_13
	.type	__vector_13, @function
__vector_13:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	lds r24,timer1_ovfs
	lds r25,timer1_ovfs+1
	adiw r24,1
	sts timer1_ovfs+1,r25
	sts timer1_ovfs,r24
	lds r24,timer1_ovfs
	lds r25,timer1_ovfs+1
	cpi r24,58
	sbci r25,2
	brlo .L11
	ldi r24,lo8(2)
	rcall SetMode
	sts timer1_ovfs+1,__zero_reg__
	sts timer1_ovfs,__zero_reg__
.L11:
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_13, .-__vector_13
	.section	.text.SendURL,"ax",@progbits
.global	SendURL
	.type	SendURL, @function
SendURL:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,55
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 55 */
/* stack size = 57 */
.L__stack_usage = 57
	movw r24,r28
	adiw r24,1
	ldi r18,lo8(55)
	movw r30,r24
	0:
	st Z+,__zero_reg__
	dec r18
	brne 0b
	ldi r22,lo8(__c.2314)
	ldi r23,hi8(__c.2314)
	rcall strcpy_P
	lds r24,position+35
	cpi r24,lo8(83)
	brne .L14
	movw r30,r28
	adiw r30,1
	0:
	ld __tmp_reg__,Z+
	tst __tmp_reg__
	brne 0b
	sbiw r30,1
	ldi r24,lo8(45)
	ldi r25,0
	std Z+1,r25
	st Z,r24
.L14:
	ldi r20,lo8(2)
	ldi r21,0
	ldi r22,lo8(position+1)
	ldi r23,hi8(position+1)
	movw r24,r28
	adiw r24,1
	rcall strncat
	ldi r22,lo8(__c.2316)
	ldi r23,hi8(__c.2316)
	movw r24,r28
	adiw r24,1
	rcall strcat_P
	ldi r22,lo8(position+3)
	ldi r23,hi8(position+3)
	movw r24,r28
	adiw r24,1
	rcall strcat
	ldi r22,lo8(__c.2318)
	ldi r23,hi8(__c.2318)
	movw r24,r28
	adiw r24,1
	rcall strcat_P
	lds r24,position+36
	cpi r24,lo8(87)
	brne .L15
	movw r30,r28
	adiw r30,1
	0:
	ld __tmp_reg__,Z+
	tst __tmp_reg__
	brne 0b
	sbiw r30,1
	ldi r24,lo8(45)
	ldi r25,0
	std Z+1,r25
	st Z,r24
.L15:
	ldi r20,lo8(2)
	ldi r21,0
	ldi r22,lo8(position+15)
	ldi r23,hi8(position+15)
	movw r24,r28
	adiw r24,1
	rcall strncat
	ldi r22,lo8(__c.2320)
	ldi r23,hi8(__c.2320)
	movw r24,r28
	adiw r24,1
	rcall strcat_P
	ldi r22,lo8(position+17)
	ldi r23,hi8(position+17)
	movw r24,r28
	adiw r24,1
	rcall strcat
	ldi r24,lo8(__c.2322)
	ldi r25,hi8(__c.2322)
	rcall SUART_PutStrFl
	ldi r24,lo8(myTelephone)
	ldi r25,hi8(myTelephone)
	rcall SUART_PutStrFl
	ldi r24,lo8(34)
	rcall SUART_PutChar
	ldi r24,lo8(13)
	rcall SUART_PutChar
	ldi r31,lo8(159999)
	ldi r18,hi8(159999)
	ldi r24,hlo8(159999)
	1: subi r31,1
	sbci r18,0
	sbci r24,0
	brne 1b
	rjmp .
	nop
	movw r24,r28
	adiw r24,1
	rcall SUART_PutStr
	ldi r24,lo8(26)
	rcall SUART_PutChar
/* epilogue start */
	adiw r28,55
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	ret
	.size	SendURL, .-SendURL
	.section	.text.SendPosition,"ax",@progbits
.global	SendPosition
	.type	SendPosition, @function
SendPosition:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(__c.2326)
	ldi r25,hi8(__c.2326)
	rcall SUART_PutStrFl
	ldi r24,lo8(myTelephone)
	ldi r25,hi8(myTelephone)
	rcall SUART_PutStrFl
	ldi r24,lo8(34)
	rcall SUART_PutChar
	ldi r24,lo8(13)
	rcall SUART_PutChar
	ldi r18,lo8(159999)
	ldi r24,hi8(159999)
	ldi r25,hlo8(159999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	lds r24,position+35
	cpi r24,lo8(83)
	brne .L17
	ldi r24,lo8(45)
	rcall SUART_PutChar
.L17:
	ldi r24,lo8(position+1)
	ldi r25,hi8(position+1)
	rcall SUART_PutStr
	ldi r24,lo8(59)
	rcall SUART_PutChar
	lds r24,position+36
	cpi r24,lo8(87)
	brne .L18
	ldi r24,lo8(45)
	rcall SUART_PutChar
.L18:
	ldi r24,lo8(position+14)
	ldi r25,hi8(position+14)
	rcall SUART_PutStr
	ldi r24,lo8(26)
	rcall SUART_PutChar
	rcall SUART_FlushInBuf
	ret
	.size	SendPosition, .-SendPosition
	.section	.text.SendSpeed,"ax",@progbits
.global	SendSpeed
	.type	SendSpeed, @function
SendSpeed:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(__c.2330)
	ldi r25,hi8(__c.2330)
	rcall SUART_PutStrFl
	ldi r24,lo8(myTelephone)
	ldi r25,hi8(myTelephone)
	rcall SUART_PutStrFl
	ldi r24,lo8(34)
	rcall SUART_PutChar
	ldi r24,lo8(13)
	rcall SUART_PutChar
	ldi r18,lo8(159999)
	ldi r24,hi8(159999)
	ldi r25,hlo8(159999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	ldi r24,lo8(position+27)
	ldi r25,hi8(position+27)
	rcall SUART_PutStr
	ldi r24,lo8(26)
	rcall SUART_PutChar
	rcall SUART_FlushInBuf
	ret
	.size	SendSpeed, .-SendSpeed
	.section	.text.ProcessGPS,"ax",@progbits
.global	ProcessGPS
	.type	ProcessGPS, @function
ProcessGPS:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r24,currentMode
	cpi r24,lo8(3)
	breq .L20
	rcall serialAvailable
	tst r24
	breq .L20
	ldi r20,lo8(13)
	ldi r22,lo8(90)
	ldi r24,lo8(gpsBuf)
	ldi r25,hi8(gpsBuf)
	rcall serialReadUntil
	sts gpsBufCnt,r24
	lds r30,gpsBufCnt
	ldi r31,0
	subi r30,lo8(-(gpsBuf))
	sbci r31,hi8(-(gpsBuf))
	st Z,__zero_reg__
	ldi r22,lo8(position)
	ldi r23,hi8(position)
	ldi r24,lo8(gpsBuf)
	ldi r25,hi8(gpsBuf)
	rcall parseGPRMC
	tst r24
	breq .L22
	lds r24,currentMode
	cpi r24,lo8(2)
	brne .L23
	ldi r24,lo8(3)
	rcall SetMode
.L23:
	lds r24,needToSendPos
	tst r24
	breq .L24
	sts needToSendPos,__zero_reg__
	rcall SendPosition
.L24:
	lds r24,needToSendSpeed
	tst r24
	breq .L22
	sts needToSendSpeed,__zero_reg__
	rcall SendSpeed
.L22:
	rcall serialClearBuffer
.L20:
	ret
	.size	ProcessGPS, .-ProcessGPS
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"+CLIP"
.LC1:
	.string	"+CMT"
.LC2:
	.string	"Spy"
.LC3:
	.string	"Off"
.LC4:
	.string	"Speed"
	.section	.text.main,"ax",@progbits
.global	main
	.type	main, @function
main:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,16
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 16 */
/* stack size = 18 */
.L__stack_usage = 18
	sbi 0xa,2
	rcall SUART_Init
	ldi r24,lo8(51)
	rcall serialInit
/* #APP */
 ;  204 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(2)
	rcall SetMode
	ldi r17,lo8(3)
.L27:
	rcall GSM_Test
	sts gsm_ok,r24
	lds r24,gsm_ok
	cpse r24,__zero_reg__
	rjmp .L26
	ldi r18,lo8(1599999)
	ldi r24,hi8(1599999)
	ldi r25,hlo8(1599999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	subi r17,lo8(-(-1))
	brne .L27
.L26:
	lds r24,gsm_ok
	cpse r24,__zero_reg__
	rjmp .L28
	rcall GSM_PowerOn
	ldi r18,lo8(11199999)
	ldi r24,hi8(11199999)
	ldi r25,hlo8(11199999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
.L28:
	rcall GSM_Init
	rcall SUART_FlushInBuf
	clr r13
	inc r13
.L42:
	rcall ProcessGPS
	rjmp .L30
.L41:
	rcall SUART_GetChar
	std Y+1,r24
	ldd r24,Y+1
	cpi r24,lo8(10)
	breq .+2
	rjmp .L31
	lds r30,gsmBufCnt
	ldi r31,0
	subi r30,lo8(-(gsmBuf))
	sbci r31,hi8(-(gsmBuf))
	st Z,__zero_reg__
	sts gsmBufCnt,__zero_reg__
	ldi r22,lo8(.LC0)
	ldi r23,hi8(.LC0)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr
	or r24,r25
	breq .L32
	ldi r22,lo8(34)
	ldi r23,0
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strchr
	movw r16,r24
	movw r14,r24
	ldi r18,-1
	sub r14,r18
	sbc r15,r18
	ldi r22,lo8(34)
	ldi r23,0
	movw r24,r14
	rcall strchr
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L32
	sbiw r24,0
	breq .L32
	movw r20,r24
	sub r20,r16
	sbc r21,r17
	movw r22,r14
	movw r24,r28
	adiw r24,2
	rcall strlcpy
	ldi r22,lo8(myTelephone)
	ldi r23,hi8(myTelephone)
	movw r24,r28
	adiw r24,2
	rcall strcmp_P
	or r24,r25
	brne .L32
	sts needToSendPos,r13
	lds r24,currentMode
	cpi r24,lo8(3)
	brne .L33
	ldi r24,lo8(2)
	rcall SetMode
.L33:
	ldi r24,lo8(__c.2344)
	ldi r25,hi8(__c.2344)
	rcall SUART_PutStrFl
	ldi r24,lo8(2399999)
	ldi r25,hi8(2399999)
	ldi r18,hlo8(2399999)
	1: subi r24,1
	sbci r25,0
	sbci r18,0
	brne 1b
	rjmp .
	nop
	rcall SUART_FlushInBuf
	rjmp .L42
.L32:
	ldi r22,lo8(.LC1)
	ldi r23,hi8(.LC1)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr
	or r24,r25
	brne .+2
	rjmp .L30
	ldi r22,lo8(34)
	ldi r23,0
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strchr
	movw r16,r24
	movw r14,r24
	ldi r24,-1
	sub r14,r24
	sbc r15,r24
	ldi r22,lo8(34)
	ldi r23,0
	movw r24,r14
	rcall strchr
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .+2
	rjmp .L30
	sbiw r24,0
	brne .+2
	rjmp .L30
	movw r20,r24
	sub r20,r16
	sbc r21,r17
	movw r22,r14
	movw r24,r28
	adiw r24,2
	rcall strlcpy
	ldi r22,lo8(myTelephone)
	ldi r23,hi8(myTelephone)
	movw r24,r28
	adiw r24,2
	rcall strcmp_P
	or r24,r25
	breq .+2
	rjmp .L30
	sts gsmBufCnt,__zero_reg__
.L37:
	rcall SUART_GetChar
	std Y+1,r24
	lds r30,gsmBufCnt
	ldd r24,Y+1
	ldi r31,0
	subi r30,lo8(-(gsmBuf))
	sbci r31,hi8(-(gsmBuf))
	st Z,r24
	lds r24,gsmBufCnt
	subi r24,lo8(-(1))
	sts gsmBufCnt,r24
	ldd r24,Y+1
	cpi r24,lo8(10)
	brne .L37
	lds r30,gsmBufCnt
	ldi r31,0
	subi r30,lo8(-(gsmBuf))
	sbci r31,hi8(-(gsmBuf))
	st Z,__zero_reg__
	ldi r22,lo8(.LC2)
	ldi r23,hi8(.LC2)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr
	or r24,r25
	breq .L38
	mov r24,r13
	rcall SetMode
	rcall SUART_FlushInBuf
	rjmp .L42
.L38:
	ldi r22,lo8(.LC3)
	ldi r23,hi8(.LC3)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr
	or r24,r25
	breq .L39
	ldi r24,lo8(3)
	rcall SetMode
	rcall SUART_FlushInBuf
	rjmp .L42
.L39:
	ldi r22,lo8(.LC4)
	ldi r23,hi8(.LC4)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr
	or r24,r25
	breq .L30
	sts needToSendSpeed,r13
	lds r24,currentMode
	cpi r24,lo8(3)
	brne .L40
	ldi r24,lo8(2)
	rcall SetMode
.L40:
	ldi r25,lo8(2399999)
	ldi r18,hi8(2399999)
	ldi r24,hlo8(2399999)
	1: subi r25,1
	sbci r18,0
	sbci r24,0
	brne 1b
	rjmp .
	nop
	rcall SUART_FlushInBuf
	rjmp .L42
.L31:
	lds r30,gsmBufCnt
	ldd r24,Y+1
	ldi r31,0
	subi r30,lo8(-(gsmBuf))
	sbci r31,hi8(-(gsmBuf))
	st Z,r24
	lds r24,gsmBufCnt
	subi r24,lo8(-(1))
	sts gsmBufCnt,r24
.L30:
	rcall SUART_Kbhit
	cpse r24,__zero_reg__
	rjmp .L41
	rjmp .L42
	.size	main, .-main
	.section	.progmem.data.__c.2344,"a",@progbits
	.type	__c.2344, @object
	.size	__c.2344, 5
__c.2344:
	.string	"ATH\r"
	.section	.progmem.data.__c.2330,"a",@progbits
	.type	__c.2330, @object
	.size	__c.2330, 11
__c.2330:
	.string	" AT+CMGS=\""
	.section	.progmem.data.__c.2326,"a",@progbits
	.type	__c.2326, @object
	.size	__c.2326, 11
__c.2326:
	.string	" AT+CMGS=\""
	.section	.progmem.data.__c.2322,"a",@progbits
	.type	__c.2322, @object
	.size	__c.2322, 11
__c.2322:
	.string	" AT+CMGS=\""
	.section	.progmem.data.__c.2320,"a",@progbits
	.type	__c.2320, @object
	.size	__c.2320, 2
__c.2320:
	.string	"+"
	.section	.progmem.data.__c.2318,"a",@progbits
	.type	__c.2318, @object
	.size	__c.2318, 4
__c.2318:
	.string	"%2C"
	.section	.progmem.data.__c.2316,"a",@progbits
	.type	__c.2316, @object
	.size	__c.2316, 2
__c.2316:
	.string	"+"
	.section	.progmem.data.__c.2314,"a",@progbits
	.type	__c.2314, @object
	.size	__c.2314, 21
__c.2314:
	.string	"http://google.ru/#q="
	.section	.bss.gsmBufCnt,"aw",@nobits
	.type	gsmBufCnt, @object
	.size	gsmBufCnt, 1
gsmBufCnt:
	.zero	1
	.section	.bss.gsmBuf,"aw",@nobits
	.type	gsmBuf, @object
	.size	gsmBuf, 90
gsmBuf:
	.zero	90
	.section	.bss.gpsBufCnt,"aw",@nobits
	.type	gpsBufCnt, @object
	.size	gpsBufCnt, 1
gpsBufCnt:
	.zero	1
	.section	.bss.gpsBuf,"aw",@nobits
	.type	gpsBuf, @object
	.size	gpsBuf, 90
gpsBuf:
	.zero	90
.global	myTelephone
	.section	.progmem.data.myTelephone,"a",@progbits
	.type	myTelephone, @object
	.size	myTelephone, 13
myTelephone:
	.string	"+79512962477"
	.section	.bss.position,"aw",@nobits
	.type	position, @object
	.size	position, 37
position:
	.zero	37
	.section	.bss.gsm_ok,"aw",@nobits
	.type	gsm_ok, @object
	.size	gsm_ok, 1
gsm_ok:
	.zero	1
	.section	.bss.needToSendSpeed,"aw",@nobits
	.type	needToSendSpeed, @object
	.size	needToSendSpeed, 1
needToSendSpeed:
	.zero	1
	.section	.bss.needToSendPos,"aw",@nobits
	.type	needToSendPos, @object
	.size	needToSendPos, 1
needToSendPos:
	.zero	1
.global	timer2_ovfs
	.section	.bss.timer2_ovfs,"aw",@nobits
	.type	timer2_ovfs, @object
	.size	timer2_ovfs, 2
timer2_ovfs:
	.zero	2
.global	timer1_ovfs
	.section	.bss.timer1_ovfs,"aw",@nobits
	.type	timer1_ovfs, @object
	.size	timer1_ovfs, 2
timer1_ovfs:
	.zero	2
	.section	.bss.currentMode,"aw",@nobits
	.type	currentMode, @object
	.size	currentMode, 1
currentMode:
	.zero	1
	.comm	lastChar,1,1
	.comm	_buffer_length,1,1
	.comm	_buffer,90,1
	.comm	_serial_initialized,1,1
	.ident	"GCC: (GNU) 4.8.2"
.global __do_copy_data
.global __do_clear_bss
