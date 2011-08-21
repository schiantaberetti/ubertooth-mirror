/*
 * Copyright 2010, 2011 Michael Ossmann
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

#ifndef __UBERTOOTH_H
#define __UBERTOOTH_H

#include "lpc17.h"
#include "types.h"
#include "cc2400.h"

/* GPIO pins */
#ifdef UBERTOOTH_ZERO
#define PIN_USRLED (1 << 11) /* P0.11 */
#define PIN_RXLED  (1 << 28) /* P4.28 */
#define PIN_TXLED  (1 << 29) /* P4.29 */
#define PIN_CC1V8  (1 << 29) /* P1.29 */
#define PIN_CC3V3  (1 << 0 ) /* P1.0  */
#define PIN_RX     (1 << 1 ) /* P1.1  */
#define PIN_TX     (1 << 4 ) /* P1.4  */
#define PIN_CSN    (1 << 8 ) /* P1.8  */
#define PIN_SCLK   (1 << 9 ) /* P1.9  */
#define PIN_MOSI   (1 << 10) /* P1.10 */
#define PIN_MISO   (1 << 14) /* P1.14 */
#define PIN_GIO6   (1 << 15) /* P1.15 */
#define PIN_BTGR   (1 << 31) /* P1.31 */
#define PIN_SSEL0  (1 << 9 ) /* P2.9  */
#endif
#ifdef UBERTOOTH_ONE
#define PIN_USRLED (1 << 1 ) /* P1.1  */
#define PIN_RXLED  (1 << 4 ) /* P1.4  */
#define PIN_TXLED  (1 << 8 ) /* P1.8  */
#define PIN_CC1V8  (1 << 9 ) /* P1.9  */
#define PIN_CC3V3  (1 << 14) /* P1.14 */
#define PIN_RX     (1 << 15) /* P1.15 */
#define PIN_TX     (1 << 29) /* P4.29 */
#define PIN_CSN    (1 << 5 ) /* P2.5  */
#define PIN_SCLK   (1 << 4 ) /* P2.4  */
#define PIN_MOSI   (1 << 0 ) /* P2.0  */
#define PIN_MISO   (1 << 1 ) /* P2.1  */
#define PIN_GIO6   (1 << 2 ) /* P2.2  */
#define PIN_BTGR   (1 << 10) /* P1.10 */
#define PIN_SSEL1  (1 << 28) /* P4.28 */
#define PIN_PAEN   (1 << 7 ) /* P2.7 */
#define PIN_HGM    (1 << 8 ) /* P2.8 */
#endif
#ifdef BROCCOLI
#define PIN_LED1     (1 << 11) /* P0.11 aka TXLED */
#define PIN_LED2     (1 << 10) /* P0.10 */
#define PIN_LED3     (1 << 1 ) /* P0.1 */
#define PIN_LED4     (1 << 0 ) /* P0.0 */
#define PIN_LED5     (1 << 29) /* P1.29 */
#define PIN_LED6     (1 << 28) /* P1.28 */
#define PIN_LED7     (1 << 26) /* P1.26 aka LED7 */
#define PIN_LED8     (1 << 25) /* P1.25 */
#define PIN_LED9     (1 << 24) /* P1.24 */
#define PIN_LED10    (1 << 23) /* P1.23 */
#define PIN_LED11    (1 << 22) /* P1.22 */
#define PIN_LED12    (1 << 20) /* P1.20 */
#define PIN_LED13    (1 << 19) /* P1.19 aka USRLED */
#define PIN_USRLED   (1 << 19) /* P1.19 aka LED13 */
#define PIN_RXLED    (1 << 26) /* P1.26 aka LED7 */
#define PIN_TXLED    (1 << 11) /* P0.11 aka LED1 */
#define PIN_VBUS     (1 << 30) /* P1.30 */
#define PIN_DIGITAL2 (1 << 0 ) /* P2.0 */
#define PIN_DIGITAL3 (1 << 1 ) /* P2.1 */
#define PIN_DIGITAL4 (1 << 2 ) /* P2.2 */
#define PIN_DIGITAL5 (1 << 3 ) /* P2.3 */
#define PIN_DIGITAL6 (1 << 4 ) /* P2.4 */
#define PIN_DIGITAL7 (1 << 5 ) /* P2.5 */
#define PIN_DIGITAL8 (1 << 6 ) /* P2.6 */
#define PIN_DIGITAL9 (1 << 7 ) /* P2.7 */
#define PIN_SW1      (1 << 8 ) /* P2.8 */
#define PIN_ANALOG2  (1 << 31) /* P1.31 */
#define PIN_ANALOG3  (1 << 25) /* P0.25 aka RSSIA */
#define PIN_ANALOG4  (1 << 26) /* P0.26 */
#define PIN_PMODE    (1 << 9 ) /* P1.9 */
#define PIN_CS       (1 << 10) /* P1.10 */
#define PIN_MODE     (1 << 29) /* P4.29 */
#define PIN_RSSID    (1 << 15) /* P1.15 */
#define PIN_INT      (1 << 14) /* P1.14 */
#define PIN_RSSIA    (1 << 25) /* P0.25 aka ANALOG3 */
#define PIN_SSEL1    (1 << 28) /* P4.28 */
#endif

/* indicator LED control */
#ifdef UBERTOOTH_ZERO
#define USRLED     (FIO0PIN & PIN_USRLED)
#define USRLED_SET (FIO0SET = PIN_USRLED)
#define USRLED_CLR (FIO0CLR = PIN_USRLED)
#define RXLED      (FIO4PIN & PIN_RXLED)
#define RXLED_SET  (FIO4SET = PIN_RXLED)
#define RXLED_CLR  (FIO4CLR = PIN_RXLED)
#define TXLED      (FIO4PIN & PIN_TXLED)
#define TXLED_SET  (FIO4SET = PIN_TXLED)
#define TXLED_CLR  (FIO4CLR = PIN_TXLED)
#endif
#ifdef UBERTOOTH_ONE
#define USRLED     (FIO1PIN & PIN_USRLED)
#define USRLED_SET (FIO1SET = PIN_USRLED)
#define USRLED_CLR (FIO1CLR = PIN_USRLED)
#define RXLED      (FIO1PIN & PIN_RXLED)
#define RXLED_SET  (FIO1SET = PIN_RXLED)
#define RXLED_CLR  (FIO1CLR = PIN_RXLED)
#define TXLED      (FIO1PIN & PIN_TXLED)
#define TXLED_SET  (FIO1SET = PIN_TXLED)
#define TXLED_CLR  (FIO1CLR = PIN_TXLED)
#endif
#ifdef BROCCOLI
#define USRLED     (FIO1PIN & PIN_USRLED)
#define USRLED_SET (FIO1SET = PIN_USRLED)
#define USRLED_CLR (FIO1CLR = PIN_USRLED)
#define RXLED      (FIO1PIN & PIN_RXLED)
#define RXLED_SET  (FIO1SET = PIN_RXLED)
#define RXLED_CLR  (FIO1CLR = PIN_RXLED)
#define TXLED      (FIO0PIN & PIN_TXLED)
#define TXLED_SET  (FIO0SET = PIN_TXLED)
#define TXLED_CLR  (FIO0CLR = PIN_TXLED)
#define LED1       (FIO0PIN & PIN_LED1)
#define LED1_SET   (FIO0SET = PIN_LED1)
#define LED1_CLR   (FIO0CLR = PIN_LED1)
#define LED2       (FIO0PIN & PIN_LED2)
#define LED2_SET   (FIO0SET = PIN_LED2)
#define LED2_CLR   (FIO0CLR = PIN_LED2)
#define LED3       (FIO0PIN & PIN_LED3)
#define LED3_SET   (FIO0SET = PIN_LED3)
#define LED3_CLR   (FIO0CLR = PIN_LED3)
#define LED4       (FIO0PIN & PIN_LED4)
#define LED4_SET   (FIO0SET = PIN_LED4)
#define LED4_CLR   (FIO0CLR = PIN_LED4)
#define LED5       (FIO1PIN & PIN_LED5)
#define LED5_SET   (FIO1SET = PIN_LED5)
#define LED5_CLR   (FIO1CLR = PIN_LED5)
#define LED6       (FIO1PIN & PIN_LED6)
#define LED6_SET   (FIO1SET = PIN_LED6)
#define LED6_CLR   (FIO1CLR = PIN_LED6)
#define LED7       (FIO1PIN & PIN_LED7)
#define LED7_SET   (FIO1SET = PIN_LED7)
#define LED7_CLR   (FIO1CLR = PIN_LED7)
#define LED8       (FIO1PIN & PIN_LED8)
#define LED8_SET   (FIO1SET = PIN_LED8)
#define LED8_CLR   (FIO1CLR = PIN_LED8)
#define LED9       (FIO1PIN & PIN_LED9)
#define LED9_SET   (FIO1SET = PIN_LED9)
#define LED9_CLR   (FIO1CLR = PIN_LED9)
#define LED10      (FIO1PIN & PIN_LED10)
#define LED10_SET  (FIO1SET = PIN_LED10)
#define LED10_CLR  (FIO1CLR = PIN_LED10)
#define LED11      (FIO1PIN & PIN_LED11)
#define LED11_SET  (FIO1SET = PIN_LED11)
#define LED11_CLR  (FIO1CLR = PIN_LED11)
#define LED12      (FIO1PIN & PIN_LED12)
#define LED12_SET  (FIO1SET = PIN_LED12)
#define LED12_CLR  (FIO1CLR = PIN_LED12)
#define LED13      (FIO1PIN & PIN_LED13)
#define LED13_SET  (FIO1SET = PIN_LED13)
#define LED13_CLR  (FIO1CLR = PIN_LED13)
#endif

/* SW1 button press */
#ifdef BROCCOLI
#define SW1 (!(FIO2PIN & PIN_SW1))
#endif

/* SSEL (SPI slave select) control for CC2400 DIO (un-buffered) serial */
#ifdef UBERTOOTH_ZERO
#define DIO_SSEL_SET  (FIO2SET = PIN_SSEL0)
#define DIO_SSEL_CLR  (FIO2CLR = PIN_SSEL0)
#endif
#if defined UBERTOOTH_ONE || defined BROCCOLI
#define DIO_SSEL_SET  (FIO4SET = PIN_SSEL1)
#define DIO_SSEL_CLR  (FIO4CLR = PIN_SSEL1)
#endif

/* 1V8 regulator control */
#if defined UBERTOOTH_ZERO || defined UBERTOOTH_ONE
#define CC1V8      (FIO1PIN & PIN_CC1V8)
#define CC1V8_SET  (FIO1SET = PIN_CC1V8)
#define CC1V8_CLR  (FIO1CLR = PIN_CC1V8)
#endif

/* CC2400 control */
#ifdef UBERTOOTH_ZERO
#define CC3V3_SET  (FIO1SET = PIN_CC3V3)
#define CC3V3_CLR  (FIO1CLR = PIN_CC3V3)
#define RX_SET     (FIO1SET = PIN_RX)
#define RX_CLR     (FIO1CLR = PIN_RX)
#define TX_SET     (FIO1SET = PIN_TX)
#define TX_CLR     (FIO1CLR = PIN_TX)
#define CSN_SET    (FIO1SET = PIN_CSN)
#define CSN_CLR    (FIO1CLR = PIN_CSN)
#define SCLK_SET   (FIO1SET = PIN_SCLK)
#define SCLK_CLR   (FIO1CLR = PIN_SCLK)
#define MOSI_SET   (FIO1SET = PIN_MOSI)
#define MOSI_CLR   (FIO1CLR = PIN_MOSI)
#define GIO6_SET   (FIO1SET = PIN_GIO6)
#define GIO6_CLR   (FIO1CLR = PIN_GIO6)
#define BTGR_SET   (FIO1SET = PIN_BTGR)
#define BTGR_CLR   (FIO1CLR = PIN_BTGR)
#define MISO       (FIO1PIN & PIN_MISO)
#endif
#ifdef UBERTOOTH_ONE
#define CC3V3_SET  (FIO1SET = PIN_CC3V3)
#define CC3V3_CLR  (FIO1CLR = PIN_CC3V3)
#define RX_SET     (FIO1SET = PIN_RX)
#define RX_CLR     (FIO1CLR = PIN_RX)
#define TX_SET     (FIO4SET = PIN_TX)
#define TX_CLR     (FIO4CLR = PIN_TX)
#define CSN_SET    (FIO2SET = PIN_CSN)
#define CSN_CLR    (FIO2CLR = PIN_CSN)
#define SCLK_SET   (FIO2SET = PIN_SCLK)
#define SCLK_CLR   (FIO2CLR = PIN_SCLK)
#define MOSI_SET   (FIO2SET = PIN_MOSI)
#define MOSI_CLR   (FIO2CLR = PIN_MOSI)
#define GIO6_SET   (FIO2SET = PIN_GIO6)
#define GIO6_CLR   (FIO2CLR = PIN_GIO6)
#define BTGR_SET   (FIO1SET = PIN_BTGR)
#define BTGR_CLR   (FIO1CLR = PIN_BTGR)
#define MISO       (FIO2PIN & PIN_MISO)
#endif

/* TRC104 control */
#ifdef BROCCOLI
#define PMODE_SET  (FIO1SET = PIN_PMODE)
#define PMODE_CLR  (FIO1CLR = PIN_PMODE)
#define CS_SET     (FIO1SET = PIN_CS)
#define CS_CLR     (FIO1CLR = PIN_CS)
#define MODE_SET   (FIO4SET = PIN_MODE)
#define MODE_CLR   (FIO4CLR = PIN_MODE)
#define RSSID      (FIO1PIN & PIN_RSSID)
#define TRC104_INT (FIO1PIN & PIN_INT)
#endif

/*
 * DIO_SSP is the SSP assigned to the CC2400's secondary ("un-buffered") serial
 * interface
 */
#ifdef UBERTOOTH_ZERO
#define DIO_SSP_CR0   SSP0CR0
#define DIO_SSP_CR1   SSP0CR1
#define DIO_SSP_DR    SSP0DR
#define DIO_SSP_DMACR SSP0DMACR
#define DIO_SSP_SRC   (1 << 1) /* for DMACCxConfig register */
#endif
#if defined UBERTOOTH_ONE || BROCCOLI
#define DIO_SSP_CR0   SSP1CR0
#define DIO_SSP_CR1   SSP1CR1
#define DIO_SSP_DR    SSP1DR
#define DIO_SSP_DMACR SSP1DMACR
#define DIO_SSP_SRC   (3 << 1) /* for DMACCxConfig register */
#endif

/* CC2591 control */
#ifdef UBERTOOTH_ONE
#define PAEN     (FIO2PIN & PIN_PAEN)
#define PAEN_SET (FIO2SET = PIN_PAEN)
#define PAEN_CLR (FIO2CLR = PIN_PAEN)
#define HGM      (FIO2PIN & PIN_HGM)
#define HGM_SET  (FIO2SET = PIN_HGM)
#define HGM_CLR  (FIO2CLR = PIN_HGM)
#endif

/*
 * clock configuration
 *
 * main oscillator:  16 MHz
 * CPU clock (PLL0): 100 MHz
 * USB clock (PLL1): 48 MHz
 */
#define CCLKSEL 3
#define MSEL0 24
#define NSEL0 1
#define MSEL1 34
#define PSEL1 0

/* flash accelerator configuration */
#define FLASHTIM 0x4 /* up to 100 MHz CPU clock */

/*
 * bootloader_ctrl is a fixed memory location used for passing
 * information from the application to the bootloader across a reset
 */
extern uint32_t bootloader_ctrl;
#define DFU_MODE 0x4305BB21

void wait(u8 seconds);
void gpio_init(void);
void ubertooth_init(void);
void dio_ssp_init(void);
void atest_init(void);
void trc104_init(void);
void cc2400_init(void);
u32 cc2400_spi(u8 len, u32 data);
u16 cc2400_get(u8 reg);
void cc2400_set(u8 reg, u16 val);
u8 cc2400_get8(u8 reg);
void cc2400_set8(u8 reg, u8 val);
u8 cc2400_status(void);
u8 cc2400_strobe(u8 reg);
void cc2400_reset(void);
void clock_start(void);
void reset(void);

#endif /* __UBERTOOTH_H */
