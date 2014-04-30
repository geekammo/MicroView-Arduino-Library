/*
    MicroView Arduino Library
    Copyright (C) 2014 GeekAmmo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SPACE03_H
#define SPACE03_H

#include <avr/pgmspace.h>

static const unsigned char space03[] PROGMEM = {
	// first row defines - FONTWIDTH, FONTHEIGHT, ASCII START CHAR, TOTAL CHARACTERS, FONT MAP WIDTH HIGH, FONT MAP WIDTH LOW (2,56 meaning 256)
	16,16,48,2,0,32,	
	0xC0, 0xC0, 0xF0, 0xF0, 0x3C, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0xF0, 0xF0, 0xC0, 0xC0,
	0xC0, 0xC0, 0xF0, 0xF0, 0x3C, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0xF0, 0xF0, 0xC0, 0xC0,
	0xC3, 0xC3, 0x33, 0x33, 0xCF, 0xCF, 0x33, 0x33, 0x33, 0x33, 0xCF, 0xCF, 0x33, 0x33, 0xC3, 0xC3,
	0x33, 0x33, 0xCF, 0xCF, 0x03, 0x03, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0xCF, 0xCF, 0x33, 0x33
};
#endif
