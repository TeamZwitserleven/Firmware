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

#include "common.h" 

void ProcessInput()
{
	if (clock == MORNING) 
	{
		LIGHT_ON(led1)
		LIGHT_ON(led2)
		LIGHT_OFF(led3)
		LIGHT_ON(led4)
	} 
	else if (clock == AFTERNOON)
	{
		LIGHT_OFF(led1)
		LIGHT_OFF(led2)
		LIGHT_OFF(led3)
		LIGHT_OFF(led4)
	}
	else if (clock == EVENING)
	{
		LIGHT_OFF(led1)
		LIGHT_ON(led2)
		LIGHT_ON(led3)
		LIGHT_ON(led4)
	}
	else if (clock == NIGHT)
	{
		LIGHT_ON(led1)
		LIGHT_OFF(led2)
		LIGHT_OFF(led3)
		LIGHT_OFF(led4)
	}
}
