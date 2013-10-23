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

#include <pic16f628.h>

// PORT A
#define FBX_BIT         0x04
#define FBY_BIT         0x08
#define FBA_BIT         0x10
#define FBB_BIT         0x20
#define FBC_BIT         0x40
#define FB_IN_MASK      0x7C
#define LED_BIT         0x80

// PORT B
#define RELAY_S1        0x02    // Activate S1
#define RELAY_S2        0x01    // Activate S2
#define RELAY_OFF       0x03    // Activate none
#define RELAY_MASK      0x03    // All deactivated
#define FB1_BIT         0x04
#define FB2_BIT         0x08
#define FB3_BIT         0x10
#define FB4_BIT         0x20
#define FB5_BIT         0x40
#define FB6_BIT         0x80
#define FB_OUT_MASK     0xFC
