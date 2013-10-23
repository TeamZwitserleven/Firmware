/*
 Modelspoorgroep Venlo MGV102 Firmware

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

#include "device.h"

// --------------------------------------------------------
// Configuration bits 
// 13  CP        1 Code protection off
// 12  Unimpl    1
// 11  Unimpl    1
// 10  Unimpl    1
// 9   Unimpl    1
// 8   CPD       1  Code protection off
// 7   LVP       0  Low voltage programming disable (since no resistor found on MGV50)
// 6   BODEN     1  Brown out reset enabled
// 5   MCLRE     0 RA5 as input
// 4   FOSC2     1  INTOSC (4MHz), RA6, RA7 I/O
// 3   PWRTE     0 Powerup timer enabled
// 2   WDTE      0 Watch dog timer disable
// 0,1 FOSC1:0   00 INTOSC (4MHz)
// --------------------------------------------------------

__xdata __at 0x2007 unsigned int CONFIG = 0x3F50;

static void InitializeIO() 
{
        // Setup
        PORTA = 0xFF;
        PORTB = 0xFF;
        // Turn comparators off
        CMCON = 0x07;
        
        // Set PORTA 0..6 as INPUT, 7 as OUTPUT
        TRISA = 0x7F;
        // Set PORTB all as OUTPUT
        TRISB = 0x00;

        // Re-setup
        PORTA = 0xFF;        
        PORTB = 0xFF;
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
                PORTA |= LED_BIT;
        } else {
                PORTA &= ~LED_BIT;
        }
}


/*
Switch the relais to S1 (direction=0) or S2 (direction=1)
*/
static void SwitchRelais(int direction) 
{
        if (direction) {
                // Switch S1
                PORTB = (PORTB & ~RELAY_MASK) | RELAY_S1;
                WaitPulse();
                PORTB |= RELAY_OFF;
        } else {
                // Switch S2
                PORTB = (PORTB & ~RELAY_MASK) | RELAY_S2;
                WaitPulse();
                PORTB |= RELAY_OFF;
        }

}

// Detection is always (in hardware) active low
#define DETECTION(x) ((curFbState & (x)) == 0)

/* Main loop */
void main() 
{
        unsigned char i;
        unsigned char lastFbState;
        unsigned char curFbState;
        unsigned char detectionOnXA;
        unsigned char detectionOnYB;
        unsigned char fb;
        
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
        lastFbState = 0xFF & FB_IN_MASK;
        
        // Perform main loop for ever
	while (1) {
	        // Get state
	        curFbState = PORTA & FB_IN_MASK;
	        
	        // Copy feedbacks to output
	        fb = PORTB & RELAY_MASK;
	        if (!DETECTION(FBX_BIT)) { fb |= FB1_BIT; }
	        if (!DETECTION(FBY_BIT)) { fb |= FB2_BIT; }
	        if (!DETECTION(FBA_BIT)) { fb |= FB3_BIT; }
	        if (!DETECTION(FBB_BIT)) { fb |= FB4_BIT; }
	        if (!DETECTION(FBC_BIT)) { fb |= FB5_BIT; }
	        
	        if (curFbState != lastFbState) {	        
	                lastFbState = curFbState;
	                detectionOnXA = DETECTION(FBX_BIT) || DETECTION(FBA_BIT);
        	        detectionOnYB = DETECTION(FBY_BIT) || DETECTION(FBB_BIT);
	                
	                // Detect change in input
        	        if (detectionOnXA && detectionOnYB) {
	                        // Input on both outer ends of the reverse loop. Panic
	                        PORTB = fb;
	                } else {
	                        PORTB = fb | FB6_BIT;
	                        if (detectionOnXA) {
                	                // Switch to X
	                                SetLed(0);  
	                                SwitchRelais(0); 
	                                // Wait to led changes settle
	                                WaitMS(2);
        	                } else if (detectionOnYB) {
                	                // Switch to X
	                                SetLed(1);
	                                SwitchRelais(1); 
	                                // Wait to led changes settle
        	                        WaitMS(2);
	                        }
                        }
                }
	}
}
