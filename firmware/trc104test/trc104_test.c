/*
 * Copyright 2011 Michael Ossmann
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
	uint8_t data;
	int toggle = 0;

	gpio_init();
	LED13_SET;

	trc104_init();
	LED12_SET;

	while (1) {
		if (toggle)
			LED11_CLR;
		else
			LED11_SET;
		toggle ^= 1;
		data = trc104_read_register(TRC104_MODECHCTRL);
		//data = trc104_read_register(TRC104_DATACTRL);
		LED1_CLR;
		LED2_CLR;
		LED3_CLR;
		LED4_CLR;
		LED5_CLR;
		LED6_CLR;
		LED7_CLR;
		LED8_CLR;
		if (data & (1 << 0)) LED1_SET;
		if (data & (1 << 1)) LED2_SET;
		if (data & (1 << 2)) LED3_SET;
		if (data & (1 << 3)) LED4_SET;
		if (data & (1 << 4)) LED5_SET;
		if (data & (1 << 5)) LED6_SET;
		if (data & (1 << 6)) LED7_SET;
		if (data & (1 << 7)) LED8_SET;
		wait(1);
	}
	USRLED_SET;

	while (1);
}
