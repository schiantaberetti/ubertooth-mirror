#ifndef __RXTX_UTILS_H__
#define __RXTX_UTILS_H__ 1

#include"bt_utils.h"
#include"rxtx_utils.h"

//uint8_t usb_msg[DMA_SIZE];

void send_usb_msg(char *msg);

void send_usb_value_msg(char *msg,u8 *data,u8 length);

void send_baseband_pkt(u8* access_code,u8* data,u16 data_len);
/*send a stream of byte in the 'access_code' physical channel*/

#define SET_BLUETOOTH_REG_CONF  cc2400_set(MANAND,  0x7fff); /*mega-reset of important signal values except VGA, see cc2400 datasheet, page 63*/ \
														cc2400_set(LMTST,   0x2b22); /*current control, see cc2400 ds page 66 */ \
														cc2400_set(MDMTST0, 0x334b); /*(set 0x134b for by now bits are not inverted prior the sending) */ 	\
														cc2400_set(GRMDM,0x0101); /* unbuffered mode (otherwise set 0x801)*/ \
														cc2400_set(FREND,   0x000f); /* max power activated!!*/ \
														cc2400_set(MDMCTRL, 0x0029); /*modulation stuff */ \
														cc2400_set(FSDIV,   channel ); /* 1 MHz IF */\
														while (!(cc2400_status() & XOSC16M_STABLE));	cc2400_strobe(SFSON); \
														while (!(cc2400_status() & FS_LOCK)); 	TXLED_SET; \
														cc2400_strobe(STX); \
														//#ifdef UBERTOOTH_ONE \
																PAEN_SET; \
														//#endif 

/* count the number of 1 bits in a uint8_t */
uint8_t count_asserted_bits(uint8_t n);

u8 rcv_baseband_pkt(u8* expected_access_code, u8* data, u16 data_len);
/*receive a stream of byte by radio and return 1 if the access_code matches, 0 otherwise*/

void send_baseband_pkt_old(u8* access_code,u8* data,u16 data_len);
/*send a stream of byte in the 'access_code' physical channel*/

void forge_fhs_pkt(u8 *fhs,u8 uap,u64 bd_addr,u8 *access_code);
/*syncword mush has been calculated on bd_addr. bd_addr is the address of this (ubertooth) device.*/

#endif
