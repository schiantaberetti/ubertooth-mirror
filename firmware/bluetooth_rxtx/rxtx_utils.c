#include"bt_utils.h"
#include"rxtx_utils.h"

uint8_t usb_msg[DMA_SIZE];

void send_usb_msg(char *msg)
{
	int i=0;
	while(i<DMA_SIZE && msg[i]!='\0')
	{
		usb_msg[i] = msg[i] & 0xFF;
		i++;
	}
	if (i<DMA_SIZE)
		usb_msg[i]='\0';
	else
		usb_msg[DMA_SIZE-1] = '\0';
	enqueue(usb_msg);
}
void send_usb_value_msg(char *msg,u8 *data,u8 length)
{
	u8 i,j=0;
	while(i<DMA_SIZE && msg[i]!='\0')
	{
		usb_msg[i] = msg[i] & 0xFF;
		i++;
	}
	j=i;
	while(j<DMA_SIZE && j<i+length)
	{
		usb_msg[j]=data[j-i] & 0xFF;
		j++;
	}
	if (j<DMA_SIZE) usb_msg[j] = '\0';
	enqueue(usb_msg);
}
void send_baseband_pkt(u8* access_code,u8* data,u16 data_len)
/*send a stream of byte in the 'access_code' physical channel*/
{
	cc2400_set(MANAND,  0x7fff);
	cc2400_set(GRMDM,   0x0801); 
	cc2400_set(LMTST,   0x2b22);
	cc2400_set(MDMTST0, 0x134b); // try 0x1b4b for inverted data (otherwise 0x134b)
	cc2400_set(FSDIV,   channel);
//	cc2400_set(SYNCH,   0xf9ae);	syncword, useless
//	cc2400_set(SYNCL,   0x1584); syncword, useless
	cc2400_set(FREND,  0xa);
	cc2400_set(MDMCTRL, 0x0029);

	while (!(cc2400_status() & XOSC16M_STABLE));
	cc2400_strobe(SFSON);
	while (!(cc2400_status() & FS_LOCK));
	TXLED_SET;
#ifdef UBERTOOTH_ONE
	PAEN_SET;
#endif

	u16 j;


	while ((cc2400_get(FSMSTATE) & 0x1f) != STATE_STROBE_FS_ON);
	// transmit a packet
	for (j = 0; j < 9; j++)
		cc2400_set8(FIFOREG, access_code[j]);
	for (j = 0; j < data_len; j++)
		cc2400_set8(FIFOREG, data[j]);
	cc2400_strobe(STX);
	// sent packet, now look for repeated packet
	while ((cc2400_get(FSMSTATE) & 0x1f) != STATE_STROBE_FS_ON);
	TXLED_CLR;
	cc2400_strobe(SRFOFF);
	while ((cc2400_status() & FS_LOCK));

#ifdef UBERTOOTH_ONE
	PAEN_CLR;
#endif
/*

	while (!(cc2400_status() & XOSC16M_STABLE));
	cc2400_strobe(SFSON);
	while (!(cc2400_status() & FS_LOCK));
	TXLED_SET;
	cc2400_strobe(STX);
#ifdef UBERTOOTH_ONE
	PAEN_SET;
#endif*/
}

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
uint8_t count_asserted_bits(uint8_t n)
{
	uint8_t i = 0;
	for (i = 0; n != 0; i++)
		n &= n - 1;
	return i;
}
u8 rcv_baseband_pkt(u8* expected_access_code, u8* data, u16 data_len)
/*receive a stream of byte by radio and return 1 if the access_code matches, 0 otherwise*/
{
	u16 i=0,errors=0;
	u8 access_code[9];

	cc2400_set(FSDIV, channel - 1);
	while (!(cc2400_status() & XOSC16M_STABLE));
	cc2400_strobe(SFSON);
	while (!(cc2400_status() & FS_LOCK));
	RXLED_SET;
	while ((cc2400_get(FSMSTATE) & 0x1f) != STATE_STROBE_FS_ON);
	cc2400_strobe(SRX);
	while (!(cc2400_status() & SYNC_RECEIVED));
	
	for (i = 0; i < 9; i++)
		access_code[i] = cc2400_get8(FIFOREG);
	for (i = 0; i < data_len; i++)
		data[i] = cc2400_get8(FIFOREG);

	while ((cc2400_get(FSMSTATE) & 0x1f) != STATE_STROBE_FS_ON);
	cc2400_strobe(SRFOFF);
	while ((cc2400_status() & FS_LOCK));
#ifdef UBERTOOTH_ONE
	PAEN_CLR;
#endif
	RXLED_CLR;


	for (i=0,errors=0;i<9;i++)
		errors+=count_asserted_bits(expected_access_code[i]^access_code[i]);
	if (errors<=MAX_SYNCWORD_ERRS) return 1;
	return 0;
}
void send_baseband_pkt_old(u8* access_code,u8* data,u16 data_len)
/*send a stream of byte in the 'access_code' physical channel*/
{

	u16 i=0;
	cc2400_set(FSDIV,   channel); //working frequency
	while (!(cc2400_status() & XOSC16M_STABLE));
	cc2400_strobe(SFSON); //Activation of frequency synthesizer, from now on it is possibile to rx or tx
	while (!(cc2400_status() & FS_LOCK));
	TXLED_SET;
#ifdef UBERTOOTH_ONE
	PAEN_SET;
#endif 
	while ((cc2400_get(FSMSTATE) & 0x1f) != STATE_STROBE_FS_ON);
			// transmit a packet
	cc2400_set(FREND,0xf);
	for (i=0;i<9;i++)
		cc2400_set8(FIFOREG,access_code[i]);
	for (i = 0; data!=NULL && i < data_len; i++)
		cc2400_set8(FIFOREG, data[i]);
	cc2400_strobe(STX);

	//while ((cc2400_get(FSMSTATE) & 0x1f) != STATE_STROBE_FS_ON);
	TXLED_CLR;
	cc2400_strobe(SRFOFF); //Turned off the frequency synthesizer
	while ((cc2400_status() & FS_LOCK));
}
void forge_fhs_pkt(u8 *fhs,u8 uap,u64 bd_addr,u8 *access_code)
/*syncword mush has been calculated on bd_addr. bd_addr is the address of this (ubertooth) device.*/
{
	u8 white_state = ((clkn >> 12) & 0x1f) ^ 0x60; //xir extended with 2 MSB
	u8 header[3] = {0};
	u8 fec_header[7] = {0};
	generate_header(header,uap,0,1,1,2,0); // parms: uap seqn arqn flow type 0010 lt_addr 0
	white_state = compute_whitening(header,header,18,white_state);
	compute_fec13(fec_header,header,18);

	u8 payload[30] = {0};// (144 bit of payload + 16 bit crc) fecced at 2/3 = 160 bits
	u8 processed_load[20]; //payload plus crc
	u64 buff;
	
	buff = 0;
	bit_copy(payload,&buff,0,61,3); // page scan mode

	buff = (clkn & 0xfffffffc) >> 2;
	bit_copy(payload,&buff,3,38,26); // clock27-2 (sampled at the beginning of the transmission of the packet)

	buff = 0;
	bit_copy(payload,&buff,29,61,3); // ltaddr

	buff = 0;
	bit_copy(payload,&buff,32,40,24); // class of device

	buff = bd_addr >> 24;
	bit_copy(payload,&buff,56,40,24); // NAP e UAP
	
	buff = 2;
	bit_copy(payload,&buff,80,62,2); // Reserved
	
	buff = 0;
	bit_copy(payload,&buff,82,62,2); // Scan mode 0, i.e. Tpage_scan <= 1.28s
	
	buff = 0;
	bit_copy(payload,&buff,84,63,1); // Undefined
	
	buff = 0;
	bit_copy(payload,&buff,85,63,1); // Extended Inquiry Response flag
	
	buff = bd_addr;
	bit_copy(payload,&buff,86,40,24); // LAP
	
	buff = get_bits(access_code,4,34);
	bit_copy(payload,&buff,110,30,34); // Parity bits
	
	u16 crc = compute_crc(payload,uap,144);
	u8 i;
	for(i=0;i<18;i++)
		processed_load[19-i] = payload[i];
	processed_load[0] = crc & 0xff;
	processed_load[1] = (crc >> 8) & 0xff;

	compute_whitening(processed_load,processed_load,160,white_state); 

	compute_fec23(payload,processed_load,160);

	merge_pkt_parts(fhs,0,fec_header,payload,240);
}

