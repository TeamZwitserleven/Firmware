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
#define LIGHT_ON(mask)  { if (mask < 50) mask++; }
#define LIGHT_OFF(mask) { if (mask > 0) mask--; } 
#define SETOUTPUT(i, mask, port) { if (i < mask) { port = BIT_ON; } else { port = BIT_OFF; } }

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
static unsigned char i;
static unsigned char j;

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
			SETOUTPUT(i, led1, LED1);
			SETOUTPUT(i, led2, LED2);
			SETOUTPUT(i, led3, LED3);
			SETOUTPUT(i, led4, LED4);
			
			for (j = 0; j < 25; j++);
		}		
	}
}
