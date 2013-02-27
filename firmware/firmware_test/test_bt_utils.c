/*
 * Copyright 2013 Luca Baldesi
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

#include<stdio.h>
#include<assert.h>
#include"bt_utils.h"
typedef unsigned char     u8;
typedef unsigned short    u16;
typedef unsigned int    u32;
typedef unsigned long    u64;

void print_byte(u8 byte)
{
	u8 bit,i;
	printf("[");
	for(i=0;i<8;i++)
	{	
		bit = (byte & (1 << (7-i))) ? 1 : 0;
		printf(" %d |",bit);
	}
	printf("]\n");
}
u8 test_bit_copy()
{
	u64 big = 0x0fffffffa5;
	u8 smalls[8]={0xFF,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

//	printf(">> Test byte copy\n");
	bit_copy(smalls,&big,0,56,8);
	assert(smalls[0]==0xa5);
	smalls[0]=0xFF;

//	printf(">> Test padding\n");
	bit_copy(smalls,&big,1,56,5);
	assert(smalls[0]==0xd3);
	smalls[0]=0xFF;
	
//	printf(">> Test ceiling\n");
	bit_copy(smalls,&big,0,56,5);
	assert(smalls[0]==0xa7);
	smalls[0]=0xFF;

//	printf(">> Test padding & ceiling\n");
	bit_copy(smalls,&big,4,56,8);
	assert(smalls[0]==0xfa && smalls[1]==0x5f );
	smalls[0]=0xFF; smalls[1]=0xFF; 

	big = 0xd34a8e;
	smalls[1] = 0xa5;smalls[3] = 0xa5;smalls[5] = 0xa5;smalls[7] = 0xa5;
//	printf(">> Test long sequence\n");
	bit_copy(smalls,&big,12,44,20);
	assert(smalls[1]==0xa3);
	assert(smalls[2]==0x4a); 
	assert(smalls[3]==0x8e);
	assert(smalls[4]==0xff);

	smalls[1] = 0xa5;smalls[3] = 0xa5;smalls[5] = 0xa5;smalls[7] = 0xa5;
	smalls[0] = 0xff;smalls[2] = 0xff;smalls[4]=0xff; smalls[6]=0xff;
//	printf(">> Test complicated sequence\n");
	bit_copy(smalls,&big,13,44,20);
	assert(smalls[1]==0xa1);
	assert(smalls[2]==0xa5);
	assert(smalls[3]==0x47);
	assert(smalls[4]==0x7f);


	big=0xa5;
	smalls[0]=0; smalls[1]=0;
//	printf(">> Test brainfucker\n");
	bit_copy(smalls,&big,9,59,5);
	assert(smalls[1]==0x14);

	printf("Test of bit_copy() Successful!\n");
}
void test_reverse_byte()
{
	u8 data = 0x0f;
	assert(reverse_byte(data) == 0xf0);
	printf("Test of reverse_byte() Successful!\n");
}
void test_compute_crc()
{
	u8 uap = 0x47,i;
	u8 data[10];
	data[0] = 0x4e;
	for(i=1;i<10;i++)
		data[i] = i;
	
	assert(compute_crc(data,uap,80) == 0x6dd2);
	printf("Test of compute_crc() Successful!\n");
}
void test_compute_hec()
{
	u8 uap = 0;
	u16 header = 0x123;
	assert(compute_hec(header,uap) == 0xe1);

	uap = 0x47;
	header = 0x123;
	assert(compute_hec(header, uap) == 0x06);

	uap = 0;
	header = 0x11b;
	assert(compute_hec(header, uap) == 0x9e);

	printf("Test of compute_hec() Successful!\n");
}
void test_compute_fec13()
{
	u8 data[2];
	u8 fec[4];

	data[0] = 0xa5;
	data[1] = 0x5a;

	compute_fec13(fec,data,9);
	assert(fec[0] == 0xe3);
	assert(fec[1] == 0x81);
	assert(fec[2] == 0xc7);
	assert(fec[3] == 0);

	printf("Test of compute_fec13() Successful!\n");
}
void test_whitening_bit()
{
	u8 white_bit,state;
	state = 0x7f;
	assert(whitening_bit(&state) == 1);

	assert(state == 0x6f);
	assert(whitening_bit(&state) == 1);

	assert(state == 0x4f);
	assert(whitening_bit(&state) == 1);

	assert(state == 0x0f);
	assert(whitening_bit(&state) == 0);

	assert(state == 0x1e);
	assert(whitening_bit(&state) == 0);

	assert(state == 0x3c);
	assert(whitening_bit(&state) == 0);

	assert(state == 0x78);
	assert(whitening_bit(&state) == 1);

	assert(state == 0x61);
	assert(whitening_bit(&state) == 1);

	printf("Test of whitening_bit() Successful!\n");
}
void test_get_bit()
{
	u8 v[3] = {0},bit;

	bit = get_bit(v,0);
	assert(bit==0);

	bit = get_bit(v,23);
	assert(bit==0);

	bit = get_bit(v,9);
	assert(bit==0);

	v[0] = 0x80;
	v[2] = 0x01;
	v[1] = 0x40;

	bit = get_bit(v,0);
	assert(bit==1);

	bit = get_bit(v,23);
	assert(bit==1);

	bit = get_bit(v,9);
	assert(bit==1);

	printf("Test of get_bit() Successful!\n");
}
void test_fec23_codeword()
{
	u16 answer = fec23_codeword(0x01);
	assert(answer == 0x2c01);

	answer = fec23_codeword(0x02);
	assert(answer == 0x5802);

	answer = fec23_codeword(0x20);
	assert(answer == 0x4c20);

	printf("Test of fec23_codeword() Successful!\n");
}
void test_compute_whitening()
{
	u8 data[2];// = {0x5a,0xa5};
	data[0] = 0xa5;
	data[1] = 0x5a;
	u8 whitened[2];
	u8 state;//bit 6-1 of clock or bit 4-0 of xir extended with 1s until 7th bit
	state = 0x7f;

	compute_whitening(whitened,data,16,state);
	assert(whitened[0] == 0x28);
	assert(whitened[1] == 0x9d);
	printf("Test of compute_whitening() Successful!\n");
}
void test_set_bit()
{
	u8 v[3] = {0};

	set_bit(v,0,1);
	assert(v[0] == 0x80);	

	set_bit(v,23,1);
	assert(v[2] == 0x01);

	set_bit(v,9,1);
	assert(v[1] == 0x40);
	
	v[0]=0xff;v[1]=0xff;v[2]=0xff;

	set_bit(v,0,0);
	assert(v[0]==0x7f);

	set_bit(v,23,0);
	assert(v[2] == 0xfe);

	set_bit(v,9,0);
	assert(v[1] == 0xbf);

	printf("Test of set_bit() Successful!\n");
}
void test_complete_pkt_dh1()
{
	u8 header[3] = {0};
	u8 fec_header[7] = {0},i;
	u8 uap = 0x47;

	generate_header(header,uap,0,1,0,0x4,0x3); // parms: uap seqn arqn flow type 0100 lt_addr 011
	
	compute_fec13(fec_header,header,18);

	//DATA (MSB...LSB)
	assert(fec_header[0] == 0x00);
	assert(fec_header[1] == 0x01);
	assert(fec_header[2] == 0xf8);
	assert(fec_header[3] == 0x1c);
	assert(fec_header[4] == 0x0e);
	assert(fec_header[5] == 0x00);
	assert(fec_header[6] == 0xfc);

	u8 payload[5]; 
	for(i=0;i<5;i++)
		payload[i] = i+1;
	u8 load[8];
	init_acl_payload(load,payload,uap,5,1,2,1); // parms: uap, length of payload, flow, LLID, crc?	

	//DATA (MSB...LSB)
	assert(load[0] == 0x6c);
	assert(load[1] == 0x37);
	assert(load[2] == 0x05);
	assert(load[3] == 0x04);
	assert(load[4] == 0x03);
	assert(load[5] == 0x02);
	assert(load[6] == 0x01);
	assert(load[7] == 0x2e);

	u8 pkt[15];
	u64 buff;
	merge_pkt_parts(pkt,0,fec_header,load,64);
		
	// DATA (LSB...MSB)
	buff = get_bits(pkt,0,9);
	assert(buff == 0x1f8); // 111111000

	buff = get_bits(pkt,9,12);
	assert(buff == 0x38); // 000000111000

	buff = get_bits(pkt,21,9);
	assert(buff == 0x38); // 000111000

	buff = get_bits(pkt,30,24);
	assert(buff == 0x1f8000); // 000111111000000000000000

	buff = get_bits(pkt,54,8);
	assert(buff == 0x74); // 01110100 

	buff = get_bits(pkt,62,8);
	assert(buff == 0x80); // 10000000

	buff = get_bits(pkt,70,8);
	assert(buff == 0x40); // 01000000

	buff = get_bits(pkt,78,8);
	assert(buff == 0xc0); // 11000000

	buff = get_bits(pkt,86,8);
	assert(buff == 0x20); // 00100000

	buff = get_bits(pkt,94,8);
	assert(buff == 0xa0); // 10100000

	buff = get_bits(pkt,102,16);
	assert(buff == 0xec36); // 1110110000110110

	printf("Test of a complete dh1 packet Successful!\n");
}
void test_complete_pkt_dm1()
{
	u8 uap = 0x47;
	u8 header[3] = {0};
	u8 fec_header[7] = {0},i;

	generate_header(header,uap,0,1,0,0x3,0x3); // parms: uap seqn arqn flow type 0011 lt_addr 011
	compute_fec13(fec_header,header,18);

	//DATA (MSB...LSB)
	assert(fec_header[0] == 0x1f);
	assert(fec_header[1] == 0xfe);
	assert(fec_header[2] == 0x07);
	assert(fec_header[3] == 0x1c);
	assert(fec_header[4] == 0x01);
	assert(fec_header[5] == 0xf8);
	assert(fec_header[6] == 0xfc);

	u8 payload[5]; 
	for(i=0;i<5;i++)
		payload[i] = i+1;
	u8 load[8] = {0}; 
	u8 fec_load[14]={0};
	init_acl_payload(load,payload,uap,5,1,2,1); // parms: uap, length of payload, flow, LLID, crc?	

	compute_fec23(fec_load,load,64);

//	printf("test codeword: %04x");
	u64 codeword;
// DATA (MSB...LSB)	
	codeword = get_bits(fec_load,0,15);
	assert(codeword == 0x4406);

	codeword = get_bits(fec_load,15,15);
	assert(codeword == 0x230d);

	codeword = get_bits(fec_load,30,15);
	assert(codeword == 0x1b05);

	codeword = get_bits(fec_load,45,15);
	assert(codeword == 0x7010);

	codeword = get_bits(fec_load,60,15);
	assert(codeword == 0x3c30);

	codeword = get_bits(fec_load,75,15);
	assert(codeword == 0x6880);

	codeword = get_bits(fec_load,90,15);
	assert(codeword == 0x4d2e);


	u8 pkt[21];
	u64 buff;
	merge_pkt_parts(pkt,0,fec_header,fec_load,15*7);

	// DATA (LSB...MSB)
	buff = get_bits(pkt,0,9);
	assert(buff == 0x1f8); // 111111000 

	buff = get_bits(pkt,9,12);
	assert(buff == 0xfc0); // 111111000000

	buff = get_bits(pkt,21,9);
	assert(buff == 0x38); // 000111000

	buff = get_bits(pkt,30,24);
	assert(buff == 0xe07ff8); // 1110 0000 0111 1111 1111 1000

	buff = get_bits(pkt,54,15);
	assert(buff == 0x3a59); // 0111010010 11001

	buff = get_bits(pkt,69,15);
	assert(buff == 0x008b); // 0000000100 01011

	buff = get_bits(pkt,84,15);
	assert(buff == 0x61e); // 0000110000 11110

	buff = get_bits(pkt,99,15);
	assert(buff == 0x407); // 0000100000 00111

	buff = get_bits(pkt,114,15);
	assert(buff == 0x506c); // 1010000011 01100

	buff = get_bits(pkt,129,15);
	assert(buff == 0x5862); // 1011000011 00010

	buff = get_bits(pkt,144,15);
	assert(buff == 0x3011); // 0110000000 10001


	printf("Test of a complete dm1 packet Successful!\n");
}
void test_get_bits()
{
	u8 data[] = {0xf0, 0x0f, 0xaa};
	u64 res;
	

	res = get_bits(data,12,8);
	assert(res == 0xfa);

	res = get_bits(data,4,10);
	assert(res == 0x3);

	printf("Test of get_bits() Successful!\n");
}
void test_gen_access_code()
{
	u8 access_code[9];
	gen_access_code(access_code,0x0000009e8b33);
	assert(access_code[0] == 0x54);
	assert(access_code[1] == 0x75);
	assert(access_code[2] == 0xc5);
	assert(access_code[3] == 0x8c);
	assert(access_code[4] == 0xc7);
	assert(access_code[5] == 0x33);
	assert(access_code[6] == 0x45);
	assert(access_code[7] == 0xe7);
	assert(access_code[8] == 0x2a);
	printf("Test of get_access_code() Successful!\n");
}
void main()
{
	printf("Test suite for bt_utils.c\n");
	printf("The sample data for the bluetooth function comes from the bluetooth specification v.4\n");
	test_reverse_byte();
	test_compute_crc();
	test_compute_hec();
	test_compute_fec13();
	test_whitening_bit();
	test_bit_copy();
	test_get_bit();
	test_fec23_codeword();
	test_set_bit();
	test_compute_whitening();
	test_get_bits();
	test_complete_pkt_dh1();
	test_complete_pkt_dm1();
	test_gen_access_code();
}
