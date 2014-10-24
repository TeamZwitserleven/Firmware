/*
 BS-1 Firmware For Houses

 Copyright (C) Ewout Prangsma <ewout@prangsma.net>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <pic12f675.h>

// Configuration word
__xdata __at 0x2007 unsigned int CONFIG = 0x31c4;

#ifdef CMNGND
	#define BIT_ON 1
	#define BIT_OFF 0
#else
	#define BIT_ON 0
	#define BIT_OFF 1
#endif

#define MORNING 0x00
#define AFTERNOON 0x01
#define EVENING 0x02
#define NIGHT 0x03

#define LED1 GPIO2
#define LED2 GPIO1
#define LED3 GPIO0
#define LED4 GPIO5

#define READ_CLOCK()    (((GPIO) >> 3) & 0x03)
#define LIGHT_ON(mask)  { if (mask < 50) { mask++; } else { mask = 50; } }
#define LIGHT_OFF(mask) { if (mask > 0) mask--; } 
#define LIGHT_WELDER(mask) { mask = 255; } 
#define SETOUTPUT(i, mask, cnt, idx, port) { \
	if (mask == 255) { \
		if (cnt == 0) { \
			if (idx < sizeof(arr)-1) { idx++; } else { idx = 0; } \
			cnt = arr[idx]; \
			if ((idx & 0x01) && (cnt != 255)) { port = BIT_ON; } else { port = BIT_OFF; } \
		} else { \
			cnt--; \
		} \
	} \
	else if (i < mask) { port = BIT_ON; } else { port = BIT_OFF; } }

void InitializeIO() 
{
	// Initialize IO ports
	GPIO = 0x0;
	ANSEL = 0;		/* Digital IO */
	CMCON = 0x07;	/* Set GP2:0 to digital IO */
	TRISIO = 0x18;	/* GP3,4 input */
}

static unsigned char clock;
static unsigned char led1;
static unsigned char led2;
static unsigned char led3;
static unsigned char led4;
static unsigned char cnt1 = 0;
static unsigned char cnt2 = 0;
static unsigned char cnt3 = 0;
static unsigned char cnt4 = 0;
static unsigned char idx1 = 0;
static unsigned char idx2 = 0;
static unsigned char idx3 = 0;
static unsigned char idx4 = 0;
static unsigned char i;
static unsigned char j;
__code static unsigned char arr[] = { 
	254, 254, 200, 120, 170, 175, 250, 200, 254, 200, 
	254, 254, 200, 120, 170, 175, 250, 200, 254, 200, 
	255, 255, 254, 255, 
	120, 190, 250, 180, 200, 190, 255, 255, 255, 255,
	254, 254, 200, 120, 170, 175, 250, 200, 254, 200, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	};

/* Process clock value in this method */
void ProcessInput();

/* Main loop */
void main() 
{
	// Initialize IO
	InitializeIO();

	while (1) 
	{
		// Read first input
		clock = READ_CLOCK();
		
		// Update led masks
		ProcessInput();
		
		// Set outputs
		for (i = 0; i < 51; i++) 
		{
			SETOUTPUT(i, led1, cnt1, idx1, LED1);
			SETOUTPUT(i, led2, cnt2, idx2, LED2);
			SETOUTPUT(i, led3, cnt3, idx3, LED3);
			SETOUTPUT(i, led4, cnt4, idx4, LED4);
			
			for (j = 0; j < 25; j++);
		}		
	}
}
