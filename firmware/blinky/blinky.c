/*
 * Copyright 2010 Michael Ossmann
 *
 * This file is part of Project Ubertooth.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "ubertooth.h"

int main()
{
	gpio_init();

	while (1) {
#if defined UBERTOOTH_ZERO || defined UBERTOOTH_ONE
		USRLED_SET;
		TXLED_SET;
		RXLED_SET;
		wait(1);
		USRLED_CLR;
		TXLED_CLR;
		RXLED_CLR;
		wait(1);
#endif
#ifdef BROCCOLI
		LED1_SET;
		LED2_SET;
		LED3_SET;
		LED4_SET;
		LED5_SET;
		LED6_SET;
		LED7_SET;
		LED8_SET;
		LED9_SET;
		LED10_SET;
		LED11_SET;
		LED12_SET;
		LED13_SET;
		wait(1);
		LED1_CLR;
		LED2_CLR;
		LED3_CLR;
		LED4_CLR;
		LED5_CLR;
		LED6_CLR;
		LED7_CLR;
		LED8_CLR;
		LED9_CLR;
		LED10_CLR;
		LED11_CLR;
		LED12_CLR;
		LED13_CLR;
		wait(1);
#endif
	}
}
