/*
 Modelspoorgroep Venlo GCA102 Firmware

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

#define U1AB GP2
#define U3AB GP3
#define UxAB_MASK 0x0C
#define LED GP1
#define PANIC GP0
#define RELAY_LEFT GP4  // S1
#define RELAY_RIGHT GP5 // S2

#define SET_PANIC { PANIC = 0; }
#define SET_NO_PANIC { PANIC = 1; }

__xdata __at 0x2007 unsigned int CONFIG = 0x31c4;

static void InitializeIO() 
{
	GPIO = 0x0;
	ANSEL = 0;		/* Digital IO */
	CMCON = 0x07;	/* Set GP2:0 to digital IO */
	TRISIO = 0x0C;	/* GP2,3 input */
}

static void WaitMS(unsigned int ms) 
{
  unsigned int i;
  for( i=0; i < ms; i++) {
    unsigned int n;
    for(n=0; n < 100; n++);
  }
}


/*
Wait a single relais pulse
*/
static void WaitPulse()
{
        WaitMS(200);
}

/*
Set the led in given direction
*/
static void SetLed(int direction) 
{
        if (direction) {
			LED = 1;
        } else {
			LED = 0;
        }
}


/*
Switch the relais to S1/left (direction=0) or S2/right (direction=1)
*/
static void SwitchRelais(int direction) 
{
        if (direction) {
                // Switch S1
				RELAY_LEFT = 1;
                WaitPulse();
				RELAY_LEFT = 0;
        } else {
                // Switch S2
				RELAY_RIGHT = 1;
                WaitPulse();
				RELAY_RIGHT = 0;
        }

}

/* Main loop */
void main() 
{
        unsigned char i;
        unsigned char lastFbState;
        unsigned char curFbState;
        unsigned char detectionOnXA;
        unsigned char detectionOnYB;
        
	// Initialize
	InitializeIO();  
	
	// Initialize
	for (i = 0; i < 2; i++) {
	        SetLed(0);
	        SwitchRelais(0);
	        WaitPulse();
	        SetLed(1);
	        SwitchRelais(1);
	        WaitPulse();
	}

    // Initialize
    lastFbState = 0xFF & UxAB_MASK;
	SET_NO_PANIC;
        
        // Perform main loop for ever
	while (1) {
	        // Get state
	        curFbState = GPIO & UxAB_MASK;
	                
	        if (curFbState != lastFbState) {	        
	                lastFbState = curFbState;
	                detectionOnXA = (U3AB == 0);
        	        detectionOnYB = (U1AB == 0);
	                
	                // Detect change in input
        	        if (detectionOnXA && detectionOnYB) {
	                        // Input on both outer ends of the reverse loop. Panic
	                        SET_PANIC;
	                } else {
							SET_NO_PANIC;
	                        if (detectionOnXA) {
                	                // Switch to X
	                                SetLed(0);  
	                                SwitchRelais(0); 
	                                // Wait to led changes settle
	                                WaitMS(2);
        	                } else if (detectionOnYB) {
                	                // Switch to Y
	                                SetLed(1);
	                                SwitchRelais(1); 
	                                // Wait to led changes settle
        	                        WaitMS(2);
	                        }
                        }
                }
	}
}
