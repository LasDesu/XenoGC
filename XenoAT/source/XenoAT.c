#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <util/delay_basic.h>
#include <math.h>
#include <string.h>

#include "qCode.h"


typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

#define RELEASE
#define LOADER_ADDR 0x40D000

/* NOTE: OUT,CLK,IN must be on the same port. */
#ifdef __AVR_ATtiny85__

#define X_OUT		_BV(4)
#define X_OUT_PORT	PORTB
#define X_OUT_PIN	PINB
#define X_OUT_DDR	DDRB
#define X_CLK		_BV(2)
#define X_CLK_PORT	PORTB
#define X_CLK_PIN	PINB
#define X_CLK_DDR	DDRB
#define X_IN		_BV(3)
#define X_IN_PORT	PORTB
#define X_IN_PIN	PINB
#define X_IN_DDR	DDRB
#define X_STR		_BV(5)
#define X_STR_PORT	PORTB
#define X_STR_PIN	PINB
#define X_STR_DDR	DDRB

#define LED1_PIN	_BV(0)
#define LED2_PIN	_BV(1)

#define LED_INIT  DDRB = LED1_PIN | LED2_PIN;

// green
#define LED1_ON   PORTB &= ~LED1_PIN
#define LED1_OFF  PORTB |=  LED1_PIN
// red
#define LED2_ON   PORTB &= ~LED2_PIN
#define LED2_OFF  PORTB |=  LED2_PIN

#else

#define VER2	// VER1 VER2


#ifdef VER1
#define X_OUT 0x10
#define X_OUT_PORT PORTB
#define X_OUT_PIN PINB
#define X_OUT_DDR DDRB
#define X_CLK 0x08
#define X_CLK_PORT PORTB
#define X_CLK_PIN PINB
#define X_CLK_DDR DDRB
#define X_IN  0x20
#define X_IN_PORT  PORTB
#define X_IN_PIN  PINB
#define X_IN_DDR  DDRB
#define X_STR 0x08
#define X_STR_PORT PORTD
#define X_STR_PIN PIND
#define X_STR_DDR DDRD

// green
#define LED1_ON   PORTB &=~0x80
#define LED1_OFF  PORTB |= 0x80
// red
#define LED2_ON   PORTC &=~0x01
#define LED2_OFF  PORTC |= 0x01
#define LED_INIT  DDRB = 0x80; DDRC = 1;


#else
#define X_OUT 0x10
#define X_OUT_PORT PORTB
#define X_OUT_PIN PINB
#define X_OUT_DDR DDRB
#define X_CLK 0x20
#define X_CLK_PORT PORTB
#define X_CLK_PIN PINB
#define X_CLK_DDR DDRB
#define X_IN  0x08
#define X_IN_PORT  PORTB
#define X_IN_PIN  PINB
#define X_IN_DDR  DDRB
#define X_STR 0x02
#define X_STR_PORT PORTB
#define X_STR_PIN PINB
#define X_STR_DDR DDRB

// green
#define LED1_ON   PORTD &=~0x04
#define LED1_OFF  PORTD |= 0x04
// red
#define LED2_ON   PORTD &=~0x08
#define LED2_OFF  PORTD |= 0x08
#define LED_INIT  DDRD = 0x0C;

#endif

#endif

extern const u8 qcode[];
extern const u8* qcode_end;
extern const u8 upload[];
extern const u8* upload_end;
extern const u8 credits[];
extern const u8* credits_end;



void reset(void);
void ldelay(volatile int i);


static inline void delay(void)
{
	/*int i = 200;
	while (i--);*/
	_delay_loop_1( 100 );
}

#if !defined(RELEASE) && !defined(__AVR_ATtiny85__)

void USART_Init( unsigned int baud )
{
	UCSRA = 2;
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable Receiver and Transmitter */
	UCSRB = /* (1<<RXEN)| */ (1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (0<<USBS)|(3<<UCSZ0)|(1<<URSEL);
}

void USART_Transmit( unsigned char data )
{
	if (data == '\n')
		USART_Transmit('\r');
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}

void sputs( char* data )
{
	while (*data) USART_Transmit(*data++);
}

#else
#define USART_Init(x)
#define USART_Transmit(x)
#define sputs(x)
#endif


#ifndef RELEASE
void sputhex16(short c)
{
	char i;
	for (i=0; i<4; ++i)
		USART_Transmit("0123456789ABCDEF"[(c>>(12-(i*4)))&0xF]);
}

void sputhex8(short c)
{
	char i;
	for (i=0; i<2; ++i)
		USART_Transmit("0123456789ABCDEF"[(c>>(4-(i*4)))&0xF]);
	
	USART_Transmit(' ');
}
#else
#define sputhex16(x)		(void)(x)
#define sputhex8(x)			(void)(x)
#endif

//// xxx stuff

static unsigned char ndelay = 0;


unsigned char io(char i)
{
	unsigned char res = 0;

	if (i)
		X_OUT_PORT |=  X_OUT;
	else
		X_OUT_PORT &= ~X_OUT;
	
	X_CLK_PORT &=~X_CLK; if (!ndelay) delay();
	res = X_IN_PIN & X_IN;
	X_CLK_PORT |= X_CLK; if (!ndelay) delay();
	
	return res;
}

void send8(unsigned char c)
{
	int i = 0;
	
	while (X_STR_PIN & X_STR) if (i++ > 10000) reset();
	for (i=0; i<8; ++i)
	{
		if (X_STR_PIN & X_STR)
			reset();
		io(c & (1<<i));
	}
}

unsigned char recv8(void)
{
	unsigned char x = 0;
	int i=0;
	while (!(X_STR_PIN & X_STR)) if (i++ > 10000) reset();
	for (i=0; i<8; ++i)
	{
		if (!(X_STR_PIN & X_STR))
			reset();
		if ( io(0) )
			x |= 1 << i;
	}
	return x;
}

unsigned char io8(unsigned char c)
{
	unsigned char x = 0;
	int i;
	
	for (i=0; i<8; ++i)
	{
		if ( io(c & (1<<i)) )
			x |= 1 << i;
	}
	return x;
}

/*
unsigned char recv8_nowait(void)
{
	unsigned char x = 0;
	int i;
	for (i=0; i<8; ++i)
		x |= io(0) << i;
	return x;
}
*/

int read_mem(unsigned char *dst, long addr, int len)
{
	send8(0xff);
	send8(0);
	send8(addr >> 8);
	send8(addr);
	send8(0);
	send8(addr >> 16); // high
	send8(0);
	send8(0);
	
	send8(len);
	send8(0);
	
	int err = recv8();
	err |= recv8() << 8;
	
	if (err)
	{
		sputs("error: ");
		sputhex16(err);
		sputs("\n");
		return err;
	}
	
	while (len--)
		*dst++ = recv8();

	return 0;
}

void write_word(long address, unsigned short data)
{
	send8(0xfe);
	send8(0x00);
	send8(address >> 8);
	send8(address);
	
	send8(data);
	send8(address >> 16);
	send8(data >> 8);
	send8(0);
	
	send8(2);
	send8(0);
	
	recv8(); recv8(); recv8(); recv8();
}

void write_word_norecv(long address, unsigned short data)
{
	send8(0xfe);
	send8(0x00);
	send8(address >> 8);
	send8(address);
	
	send8(data);
	send8(address >> 16);
	send8(data >> 8);
	send8(0);
	
	send8(2);
	send8(0);
}

void write_block(long address, const unsigned char *source, int len)
{
	while (len >= 1) {
		write_word(address, pgm_read_byte(source) | (pgm_read_byte(source+1) << 8));
		address += 2;
		len -= 2;
		source += 2;
	}
} 

void reset()
{
	sputs("RESET!\n");
	WDTCR = (1<<WDE);
	while (1);
}

static void WDT_off()
{
	__builtin_avr_wdr();
	/* Clear WDRF in MCUSR */
	MCUSR = 0x00;
	/* Write logical one to WDCE and WDE */
	WDTCR = (1<<WDCE) | (1<<WDE);
	/* Turn off WDT */
	WDTCR = 0x00;
}

void ldelay(volatile int i)
{
	while (i--) {
//		if (X_STR_PIN & X_STR)
//			reset();
	}
}


/*
void sleep(int nMs)
{
	while(--nMs != 0) {
		ldelay(250);
		if (X_STR_PIN & X_STR)
			reset();
	}
}

#define BLUE	0
#define RED		1

void sleepOrg(int nMs)
{
	while(--nMs != 0) {
		ldelay(250);
	}
}

void FlashLED(u8 bLed, int nTimes)
{
	while(nTimes-->0) {
		if(bLed == 0)	LED1_ON;
		else			LED2_ON;
		sleep(10);
		if(bLed == 0)	LED1_OFF;
		else			LED2_OFF;
		sleep(10);
	}
}
*/

int main(void)
{
	const unsigned int	qcodesize	= ((((const unsigned int) &qcode_end)	- ((const  unsigned int) &qcode))	& 0xFFFE) + 2;
	const unsigned int	uploadsize	= ((((const unsigned int) &upload_end)	- ((const  unsigned int) &upload))	& 0xFFFE) + 2;
//	const unsigned int	creditssize = ((((const unsigned int) &credits_end)	- ((const  unsigned int) &credits))	& 0xFFFC) + 4;
	const unsigned int	creditssize = ((((const unsigned int) &credits_end)	- ((const  unsigned int) &credits))	& 0xFFFE) + 2;

	unsigned short last_recv=0;

	LED_INIT;
#ifndef RELEASE	
	USART_Init(103);
#endif
	
	int i;
	X_OUT_PORT &= ~(X_CLK|X_OUT|X_IN);
	X_STR_DDR &= ~X_STR;
	//X_OUT_DDR = 2 | X_CLK | X_OUT | 0x80;
	X_OUT_DDR |= X_CLK | X_OUT;
	X_STR_PORT |= X_STR;
	
	WDT_off();
	
	// VCC/GND not present: no leds
	// CLK missing: will be kept in first stage
	// DIN missing: will be kept in second stage
	// DOUT missing: will always reset
	// 1 0
	// 1 1
	// 0 0
	// 0 1
	LED2_ON; LED1_OFF; 	// -> red

#if defined(__AVR_ATtiny85__)
	// set 8MHz
	clock_prescale_set( clock_div_1 );
#endif

	sputs("syncing..\n");
	while (1) {
		last_recv >>= 1;
		if ( io(1) )
			last_recv |= 0x8000;
//		sputhex16(last_recv); sputs("\n");
		if (last_recv == 0xeeee)
			break;
	}
	sputs("sync ok.\n");

	// stack-friendly loading :p
	static const u8 PROGMEM pLoaderCode[] =
	{
		0x80, 0x00,					//  8000		MOV	$00,D0				
		0xC4, 0xDA, 0xFC,			//  C4DAFC		MOVB	D0,($FCDA)	# disable breakpoints

		0xF4,0x74,0x74,0x0a,0x08,	//	F47474A708  MOV	$080a74,a0		# restore original 
		0xF7,0x20,0x4C,0x80,		//	F7204C80    MOV	a0,($804c)		# inthandler
		0xF4,0x74,					//	F47400D040  MOV	QCODEIMGBASE,a0	# jump to drivecode init
		(LOADER_ADDR		& 0xFF),				
		(LOADER_ADDR >> 8	& 0xFF),
		(LOADER_ADDR >> 16	& 0xFF),		
		0xF0,0x00					//	F000        JMP	(a0)
	};

	const u8* pUpload = qcode;
	u16 wUploadSize = qcodesize;

	u16 wTest = 0;
	read_mem(&wTest, 0x40D100, 2);

	if(wTest == 0x4444) {
		pUpload = credits;
		wUploadSize = creditssize;
	}

	write_block(LOADER_ADDR, upload, uploadsize);
	write_block(0x8674, pLoaderCode, sizeof(pLoaderCode));
	write_word_norecv(0x804d, 0x0086);


	char u8Ret = io8(0x00);
	ndelay = 1;
	
	ldelay(100);
	u8Ret |= io8(0x00);

	ldelay(100);
	io8((wUploadSize >> 9)&0xFF);
	ldelay(100);
	io8((wUploadSize >> 1)&0xFF);
	ldelay(100);
	io8(0);
	
	LED2_OFF;
	unsigned char r, e = (wUploadSize >> 1) & 0xFF, n, csum = 0;

	for (i=0; i < wUploadSize; ++i)	{
		ldelay(100);
		r = io8(n = pgm_read_byte(pUpload + i));
		csum += n;

		if (r != e)	{
			reset();
		}

		e = n;
	}

	ldelay(100);
	pgm_read_byte(pUpload + i);

//	if(bUploadAgain) {
//		io8(0x99);
//		goto uploadAgain;
//	}
//	else 
	{
		io8(0x21);
	}

	io8(0);
	io8(0);

/*	if(pUpload == credits) {
		LED1_ON;
		LED2_ON;
		X_STR_PORT |= X_STR;
		while(1);
	}
*/
	// SUCCESS (BLUE)
	LED1_ON;
	X_STR_PORT |= X_STR;

//	if(pUpload == credits) {
//		LED1_ON;
//		LED2_ON;
//		sleepOrg(3000);
//	}

	while (!(X_STR_PIN & X_STR)) {
	}

	reset();
	return 0;
}


