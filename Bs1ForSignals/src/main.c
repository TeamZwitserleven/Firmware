/*
 Team Zwitserleven BS-1 Firmware For Signals

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
unsigned int __at 0x2007 CONFIG = 0x31c4;

#ifdef CMNGND
	#define BIT_ON 1
	#define BIT_OFF 0
#else
	#define BIT_ON 0
	#define BIT_OFF 1
#endif

#define UPDATE(input, value, mask) { if (input == value) { if (mask < 50) mask++; } else { if (mask > 0) mask--; } }
#define SETOUTPUT(i, mask, port) { if (i < mask) { port = BIT_ON; } else { port = BIT_OFF; } }

/* Main loop */
void main() 
{
	static unsigned char input;
	static unsigned char mask1;
	static unsigned char mask2;
	static unsigned char mask3;
	static unsigned char mask4;
	static unsigned char i;
	static unsigned char j;

	// Initialize IO
	GPIO = 0x0;
	ANSEL = 0;		/* Digital IO */
	CMCON = 0x07;	/* Set GP2:0 to digital IO */
	TRISIO = 0x18;	/* GP3,4 input */

	while (1) 
	{
#ifdef COLORS2
// dual 2-color signals
		// Read first input
		input = ((GPIO) >> 3) & 0x01;		
		
		// Update masks
		UPDATE(input, 0, mask1)
		UPDATE(input, 1, mask2)

		// Read second input
		input = ((GPIO) >> 4) & 0x01;		
		
		// Update masks
		UPDATE(input, 0, mask3)
		UPDATE(input, 1, mask4)
#else
// 4-color signals
		// Read input
		input = ((GPIO) >> 3) & 0x03;
		
		// Update masks
		UPDATE(input, 0, mask1)
		UPDATE(input, 1, mask2)
		UPDATE(input, 2, mask3)
		UPDATE(input, 3, mask4)
#endif
		
		// Set outputs
		for (i = 0; i < 51; i++) 
		{
			SETOUTPUT(i, mask1, GPIO2);
			SETOUTPUT(i, mask2, GPIO1);
			SETOUTPUT(i, mask3, GPIO0);
			SETOUTPUT(i, mask4, GPIO5);
			
			for (j = 0; j < 25; j++);
		}		
	}
}
