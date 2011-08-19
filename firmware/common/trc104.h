/*
 * trc104.h
 *
 * registers etc. for the RFM TRC104 wireless transceiver
 *
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

#ifndef __TRC104_H
#define __TRC104_H

/* TRC104 registers */
#define TRC104_MODECHCTRL  0x00 /* T/R Mode and Channel Frequency Control */
#define TRC104_PWRXTALCTRL 0x01 /* Transmitter Power and Crystal Frequency Control */
#define TRC104_DATACTRL    0x02 /* Data Function Control */
#define TRC104_RSSICTRL    0x03 /* RSSI Function Control */
#define TRC104_RSSIVAL     0x04 /* RSSI Value */
#define TRC104_FORMATCTRL  0x05 /* Data Format Control */
#define TRC104_PREAMBCTRL  0x06 /* Preamble Control */
#define TRC104_TIMECTRL    0x07 /* Transmitter Rise/Fall Time Control */
#define TRC104_ADDRLENCTRL 0x08 /* Address Length Control */
#define TRC104_DESTADDR1   0x09 /* Destination Address 1 */
#define TRC104_DESTADDR2   0x0A /* Destination Address 2 */
#define TRC104_DESTADDR3   0x0B /* Destination Address 3 */
#define TRC104_DESTADDR4   0x0C /* Destination Address 4 */
#define TRC104_DESTADDR5   0x0D /* Destination Address 5 */
#define TRC104_DEVADDR1    0x0E /* Sender (Local Device) Address 1 */
#define TRC104_DEVADDR2    0x0F /* Sender (Local Device) Address 2 */
#define TRC104_DEVADDR3    0x10 /* Sender (Local Device) Address 3 */
#define TRC104_DEVADDR4    0x11 /* Sender (Local Device) Address 4 */
#define TRC104_DEVADDR5    0x12 /* Sender (Local Device) Address 5 */
/* 0x13 reserved */
#define TRC104_PLLON       0x14 /* PLL Turn-on Control */
#define TRC104_ANAON       0x15 /* Analog Turn-on Control */
/* 0x16 reserved */
#define TRC104_OPTCTRL     0x17 /* Option Control */
/* 0x18 and higher reserved */

/* tuning limits */
#define MIN_FREQ 2401
#define MAX_FREQ 2527

#endif /* __TRC104_H */
