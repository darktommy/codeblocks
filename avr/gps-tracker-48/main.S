	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.section	.text.SetMode,"ax",@progbits
.global	SetMode
	.type	SetMode, @function
SetMode:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r25,currentMode
	cp r24,r25
	breq .L1
/* #APP */
 ;  45 "main.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	cpi r24,lo8(1)
	brlo .L3
	cpi r24,lo8(3)
	brlo .L4
	cpi r24,lo8(3)
	brne .L3
	cbi 0xb,2
	sts 132+1,__zero_reg__
	sts 132,__zero_reg__
	ldi r25,lo8(1)
	sts 111,r25
	sts 128,__zero_reg__
	ldi r25,lo8(4)
	sts 129,r25
	rjmp .L3
.L4:
	sbi 0xb,2
	sts 129,__zero_reg__
	sts 132+1,__zero_reg__
	sts 132,__zero_reg__
.L3:
	sts currentMode,r24
/* #APP */
 ;  63 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L1:
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
	brlo .L6
	ldi r24,lo8(2)
	rcall SetMode
	sts timer1_ovfs+1,__zero_reg__
	sts timer1_ovfs,__zero_reg__
.L6:
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
	ldi r22,lo8(__c.2308)
	ldi r23,hi8(__c.2308)
	rcall strcpy_P
	lds r24,position+35
	cpi r24,lo8(83)
	brne .L9
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
.L9:
	ldi r20,lo8(2)
	ldi r21,0
	ldi r22,lo8(position+1)
	ldi r23,hi8(position+1)
	movw r24,r28
	adiw r24,1
	rcall strncat
	ldi r22,lo8(__c.2310)
	ldi r23,hi8(__c.2310)
	movw r24,r28
	adiw r24,1
	rcall strcat_P
	ldi r22,lo8(position+3)
	ldi r23,hi8(position+3)
	movw r24,r28
	adiw r24,1
	rcall strcat
	ldi r22,lo8(__c.2312)
	ldi r23,hi8(__c.2312)
	movw r24,r28
	adiw r24,1
	rcall strcat_P
	lds r24,position+36
	cpi r24,lo8(87)
	brne .L10
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
.L10:
	ldi r20,lo8(2)
	ldi r21,0
	ldi r22,lo8(position+15)
	ldi r23,hi8(position+15)
	movw r24,r28
	adiw r24,1
	rcall strncat
	ldi r22,lo8(__c.2314)
	ldi r23,hi8(__c.2314)
	movw r24,r28
	adiw r24,1
	rcall strcat_P
	ldi r22,lo8(position+17)
	ldi r23,hi8(position+17)
	movw r24,r28
	adiw r24,1
	rcall strcat
	ldi r24,lo8(__c.2316)
	ldi r25,hi8(__c.2316)
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
	ldi r24,lo8(__c.2320)
	ldi r25,hi8(__c.2320)
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
	brne .L12
	ldi r24,lo8(45)
	rcall SUART_PutChar
.L12:
	ldi r24,lo8(position+1)
	ldi r25,hi8(position+1)
	rcall SUART_PutStr
	ldi r24,lo8(59)
	rcall SUART_PutChar
	lds r24,position+36
	cpi r24,lo8(87)
	brne .L13
	ldi r24,lo8(45)
	rcall SUART_PutChar
.L13:
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
	ldi r24,lo8(__c.2324)
	ldi r25,hi8(__c.2324)
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
 ;  151 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(2)
	rcall SetMode
	ldi r17,lo8(3)
.L17:
	rcall GSM_Test
	sts gsm_ok,r24
	lds r24,gsm_ok
	cpse r24,__zero_reg__
	rjmp .L16
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
	brne .L17
.L16:
	lds r24,gsm_ok
	cpse r24,__zero_reg__
	rjmp .L18
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
.L18:
	sts needToSendPos,__zero_reg__
	sts needToSendSpeed,__zero_reg__
	rcall GSM_Init
	clr r13
	inc r13
.L36:
	lds r24,currentMode
	cpi r24,lo8(3)
	breq .L20
	rcall serialAvailable
	tst r24
	breq .L20
	ldi r20,lo8(13)
	ldi r22,lo8(80)
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
	breq .L21
	lds r24,currentMode
	cpi r24,lo8(2)
	brne .L21
	ldi r24,lo8(3)
	rcall SetMode
.L21:
	rcall serialClearBuffer
.L20:
	lds r24,needToSendPos
	tst r24
	breq .L22
	lds r24,position
	tst r24
	breq .L22
	sts needToSendPos,__zero_reg__
	rcall SendPosition
.L22:
	lds r24,needToSendSpeed
	tst r24
	brne .+2
	rjmp .L24
	lds r24,position
	tst r24
	brne .+2
	rjmp .L24
	sts needToSendSpeed,__zero_reg__
	rcall SendSpeed
	rjmp .L24
.L35:
	rcall SUART_GetChar
	std Y+1,r24
	ldd r24,Y+1
	cpi r24,lo8(10)
	breq .+2
	rjmp .L25
	lds r30,gsmBufCnt
	ldi r31,0
	subi r30,lo8(-(gsmBuf))
	sbci r31,hi8(-(gsmBuf))
	st Z,__zero_reg__
	sts gsmBufCnt,__zero_reg__
	ldi r22,lo8(__c.2336)
	ldi r23,hi8(__c.2336)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr_P
	or r24,r25
	breq .L26
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
	breq .L26
	sbiw r24,0
	breq .L26
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
	brne .L26
	sts needToSendPos,r13
	lds r24,currentMode
	cpi r24,lo8(3)
	brne .L27
	ldi r24,lo8(2)
	rcall SetMode
.L27:
	ldi r24,lo8(__c.2338)
	ldi r25,hi8(__c.2338)
	rcall SUART_PutStrFl
	ldi r24,lo8(3199999)
	ldi r25,hi8(3199999)
	ldi r18,hlo8(3199999)
	1: subi r24,1
	sbci r25,0
	sbci r18,0
	brne 1b
	rjmp .
	nop
	rcall SUART_FlushInBuf
	rjmp .L36
.L26:
	ldi r22,lo8(__c.2341)
	ldi r23,hi8(__c.2341)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr_P
	or r24,r25
	brne .+2
	rjmp .L24
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
	rjmp .L24
	sbiw r24,0
	brne .+2
	rjmp .L24
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
	rjmp .L24
	sts gsmBufCnt,__zero_reg__
.L31:
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
	brne .L31
	lds r30,gsmBufCnt
	ldi r31,0
	subi r30,lo8(-(gsmBuf))
	sbci r31,hi8(-(gsmBuf))
	st Z,__zero_reg__
	ldi r22,lo8(__c.2345)
	ldi r23,hi8(__c.2345)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr_P
	or r24,r25
	breq .L32
	mov r24,r13
	rcall SetMode
	rcall SUART_FlushInBuf
	rjmp .L36
.L32:
	ldi r22,lo8(__c.2347)
	ldi r23,hi8(__c.2347)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr_P
	or r24,r25
	breq .L33
	ldi r24,lo8(3)
	rcall SetMode
	rcall SUART_FlushInBuf
	rjmp .L36
.L33:
	ldi r22,lo8(__c.2349)
	ldi r23,hi8(__c.2349)
	ldi r24,lo8(gsmBuf)
	ldi r25,hi8(gsmBuf)
	rcall strstr_P
	or r24,r25
	breq .L24
	sts needToSendSpeed,r13
	lds r24,currentMode
	cpi r24,lo8(3)
	brne .L34
	ldi r24,lo8(2)
	rcall SetMode
.L34:
	ldi r25,lo8(3199999)
	ldi r18,hi8(3199999)
	ldi r24,hlo8(3199999)
	1: subi r25,1
	sbci r18,0
	sbci r24,0
	brne 1b
	rjmp .
	nop
	rcall SUART_FlushInBuf
	rjmp .L36
.L25:
	lds r30,gsmBufCnt
	ldd r24,Y+1
	ldi r31,0
	subi r30,lo8(-(gsmBuf))
	sbci r31,hi8(-(gsmBuf))
	st Z,r24
	lds r24,gsmBufCnt
	subi r24,lo8(-(1))
	sts gsmBufCnt,r24
.L24:
	rcall SUART_Kbhit
	cpse r24,__zero_reg__
	rjmp .L35
	rjmp .L36
	.size	main, .-main
	.section	.progmem.data.__c.2349,"a",@progbits
	.type	__c.2349, @object
	.size	__c.2349, 6
__c.2349:
	.string	"Speed"
	.section	.progmem.data.__c.2347,"a",@progbits
	.type	__c.2347, @object
	.size	__c.2347, 4
__c.2347:
	.string	"Off"
	.section	.progmem.data.__c.2345,"a",@progbits
	.type	__c.2345, @object
	.size	__c.2345, 4
__c.2345:
	.string	"Spy"
	.section	.progmem.data.__c.2341,"a",@progbits
	.type	__c.2341, @object
	.size	__c.2341, 5
__c.2341:
	.string	"+CMT"
	.section	.progmem.data.__c.2338,"a",@progbits
	.type	__c.2338, @object
	.size	__c.2338, 6
__c.2338:
	.string	" ATH\r"
	.section	.progmem.data.__c.2336,"a",@progbits
	.type	__c.2336, @object
	.size	__c.2336, 6
__c.2336:
	.string	"+CLIP"
	.section	.progmem.data.__c.2324,"a",@progbits
	.type	__c.2324, @object
	.size	__c.2324, 11
__c.2324:
	.string	" AT+CMGS=\""
	.section	.progmem.data.__c.2320,"a",@progbits
	.type	__c.2320, @object
	.size	__c.2320, 11
__c.2320:
	.string	" AT+CMGS=\""
	.section	.progmem.data.__c.2316,"a",@progbits
	.type	__c.2316, @object
	.size	__c.2316, 11
__c.2316:
	.string	" AT+CMGS=\""
	.section	.progmem.data.__c.2314,"a",@progbits
	.type	__c.2314, @object
	.size	__c.2314, 2
__c.2314:
	.string	"+"
	.section	.progmem.data.__c.2312,"a",@progbits
	.type	__c.2312, @object
	.size	__c.2312, 4
__c.2312:
	.string	"%2C"
	.section	.progmem.data.__c.2310,"a",@progbits
	.type	__c.2310, @object
	.size	__c.2310, 2
__c.2310:
	.string	"+"
	.section	.progmem.data.__c.2308,"a",@progbits
	.type	__c.2308, @object
	.size	__c.2308, 21
__c.2308:
	.string	"http://google.ru/#q="
	.section	.bss.gsmBufCnt,"aw",@nobits
	.type	gsmBufCnt, @object
	.size	gsmBufCnt, 1
gsmBufCnt:
	.zero	1
	.section	.bss.gsmBuf,"aw",@nobits
	.type	gsmBuf, @object
	.size	gsmBuf, 80
gsmBuf:
	.zero	80
	.section	.bss.gpsBufCnt,"aw",@nobits
	.type	gpsBufCnt, @object
	.size	gpsBufCnt, 1
gpsBufCnt:
	.zero	1
	.section	.bss.gpsBuf,"aw",@nobits
	.type	gpsBuf, @object
	.size	gpsBuf, 80
gpsBuf:
	.zero	80
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
	.comm	_buffer,81,1
	.comm	_serial_initialized,1,1
	.ident	"GCC: (GNU) 4.8.2"
.global __do_clear_bss
