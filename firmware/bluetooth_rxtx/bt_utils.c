#include"bt_utils.h"

void gen_access_code(u8 *access_code,u64 address)
/*Generate the access code. The access_code array is required to have a size of at least 9 bytes. The access code computed is in the 'air' order (i.e. LSB to MSB).*/
{
	u8 i;
	/* default codeword modified for PN sequence and barker code */
	u64 codeword = 0xb0000002c7820e7e;
	
	/* the sync word generated is in host order, not air order */ 
	for (i = 0; i < 24; i++)
		if (address & (0x800000 >> i))
			codeword ^= sw_matrix[i];
	
	bit_copy(access_code,&codeword,0,0,64);
	codeword = 0;
	for(i=0;i<8;i++)
		codeword ^= ((u64)reverse_byte(access_code[7-i])) << 8*(7-i) ;

	access_code[0] = 0;
	access_code[8] = 0;
	bit_copy(access_code,&codeword,4,0,64);

	if(access_code[0] & 0x8)
		access_code[0] ^= 0xa0;
	else
		access_code[0] ^= 0x50;
	if(access_code[8] & 0x10)
		access_code[8] ^= 0x05;
	else
		access_code[8] ^= 0x0a;
}

u16 merge_pkt_parts(u8 *pkt,u8 *access_code,u8 *header, u8* payload,u16 load_bits)
/*pkt must have a dimension of at least 72+54+load_bits bit.
	Returns the number of byte over-written.
	It is important that load_bits is exactly the number of bit of interesting*/
{
	u16 i,j=0;
	u8 bit;

	if(access_code)
		for(i=0;i<72;i++)
		{
			bit = get_bit(access_code,71-i);
			set_bit(pkt,j++,bit);
		}

	if(header)
		for(i=0;i<54;i++)
		{
			bit = get_bit(header,53-i);
			//bit = get_bit(header,i);
			set_bit(pkt,j++,bit);
		}

	if(payload && load_bits)
		for(i=0;i<load_bits;i++)
		{
			bit = get_bit(payload,load_bits-1-i);
			//bit = get_bit(payload,i);
			set_bit(pkt,j++,bit);
		}

	return j/8 + (j & 0x7 ? 1 : 0);
}

u8 compute_whitening(u8 *dst,u8 *src,u16 n_bits,u8 state)
/*It returns the state of the LFSR
	Warning: this function does not reverse the bits!*/
{
	u16 i;
	u8 bit_in,white_bit;

	for (i=0;i< n_bits;i++)
	{
			bit_in = get_bit(src,n_bits-i-1);
			white_bit = whitening_bit(&state);
			set_bit(dst,n_bits-i-1,bit_in ^ white_bit);
	}
	return state;
}

void set_bit(u8 *vect,u16 bit,u8 value)
{
	if(value)
		vect[bit/8] |= (0x80 >> (bit & 0x07));
	else
		vect[bit/8] &= ~(0x80 >> (bit & 0x07));
}

u8 reverse_byte(u8 byte)
{
	u8 i,res=0;
	for (i=0;i<8;i++)
		if((byte & (1 << i)) ? 1 : 0)
			res |= (1<<(7-i));
	return res;
}
u16 compute_crc(u8 *data,u16 state,u8 nbits)
{
	u8 i,bit_in,bit_add;
	u8 non_padding_bits = nbits & 0x07;
	u8 n_bytes = nbits/8;
	u16 add_mask = 0x1021;

//	printf("State is: %04x\n",state);
	if(non_padding_bits)
	{
//		printf("Rilevati bit a cazzo\n");
		for(i=0;i<=non_padding_bits;i++)
		{
			bit_in = (data[n_bytes] & (0x80 >> (non_padding_bits-i))) ? 1 : 0;
			bit_add = bit_in ^ ((state & (0x8000)) ? 1 : 0);
			state = state << 1;	
			if(bit_add)
				state ^= add_mask;
		}
	}
	u8 j;
	for(j=0;j<n_bytes;j++)
	{
//		printf("Processing byte: %02x\n",data[j]);
		for(i=0;i<8;i++)
		{
//		printf("State is: %04x\n",state);
//		print_byte(((u8*)&state)[1]); print_byte(((u8*)&state)[0]);
			bit_in = (data[j] & (1<<i)) ? 1 : 0;
			bit_add = bit_in ^ ((state & (0x8000)) ? 1 : 0);
//			printf("BITIN: %d BITADD %d\n",bit_in,bit_add);
			state = state << 1;	
			if(bit_add)
			{
				state ^= add_mask;
			}
		}
	}
//	((u8*)&state)[1] = reverse_byte(((u8*)&state)[1]);
//	((u8*)&state)[0] = reverse_byte(((u8*)&state)[0]);

	u16 crc = 0;
	crc ^= reverse_byte(state & 0x00FF);
	crc ^= reverse_byte((state & 0xFF00)>>8) << 8;
	return crc;
}
u8 compute_hec(u16 header,u8 state)
{
	u8 i,bit_in,add_bit;
	u8 add_mask = 0xA7;

	for(i=0;i<10;i++)	
	{
		bit_in = (header & (1<<(i))) ? 1 : 0;//n-th bit in input
		add_bit = bit_in ^ ((128 & state) ? 1 : 0); //bit to add
		state = state << 1;
		if(add_bit) 
			state = state ^ add_mask;
	}
	return reverse_byte(state);
}
void generate_header(u8 *dst,u8 uap,u8 seqn,u8 arqn,u8 flow,u8 type,u8 lt_addr)
/*Generate the header of a BT packet. The dst array must have a size of 3 bytes at least.*/
{
	u16 header_info = 0;
	u8 hec;
	u64 tmp;

	if(seqn)
		header_info = 1;
	header_info << 1;
	if(arqn)
		header_info ^= 1;
	header_info = header_info << 1;
	if(flow)
		header_info ^= 1;
	header_info = header_info << 4;
	header_info ^= 0x0F & type;
	header_info = header_info << 3;
	header_info ^= 0x0F & lt_addr;
	
	hec = compute_hec(header_info,uap);
	tmp = hec;
	bit_copy(dst,&tmp,0,56,8);
	tmp = header_info;
	bit_copy(dst,&tmp,8,54,10);
}

void compute_fec13(u8 *dest,u8 *src,u8 bits)
/*Replies every bit three times*/
{
	u8 bit,i,j;
	for(i=0;i<bits;i++)
	{
		bit = get_bit(src,i);
		for(j=0;j<3;j++)
			set_bit(dest,i*3+j,bit);
	}
}
u8 whitening_bit(u8 *state)
{
	u8 add_mask=0x11;
	u8 bit = ((*state) & (1<<6)) ? 1 : 0;
	*state = (*state) << 1 ;
	*state &= 0x7f;
	if(bit)
		*state ^= add_mask;
	return bit;
}
u8 bit_copy(u8 *dst,u64 *src,u16 dst_start,u16 src_start,u16 n_bits)
/*Copy nbits from variable src into array dst. Returns 0 if everything is ok, 1 otherwise*/
{
	u16 i;
	u8 byte_index,padding,mask;
	
	byte_index = dst_start/8;
	padding = (8-(dst_start & 0x07)) & 0x07;
	u8 ceiling = n_bits > padding ? (n_bits - padding) & 0x07 : 0;
//	printf("n_bits: %d, byte_index: %d, padding: %d, ceiling: %d\n",n_bits,byte_index,padding,ceiling);

	if((n_bits+src_start)>64) return 1;
	if(padding) //devo catturare padding bit per riallinearmi
	{	
		if((8-padding+n_bits) < 8)
			mask = (0xFF>>((8-padding)+n_bits));  
		else
			mask = 0;
		mask |= (0xFF<<padding);		// mask for bits to be changed
		dst[byte_index] &= mask; 
		dst[byte_index] ^= (src_start + padding > 64) ? ((*src) << (padding-n_bits)) & ~mask : (((*src) >> (64-src_start-padding)))  & ~mask ;
		byte_index++;
	}

	for(i=src_start+padding;i+7<src_start+n_bits;i+=8) //i is the bit index in the source
	{
		dst[byte_index] = 0;
		dst[byte_index] ^= ((*src) >> (64-i-8)) & 0xFF;
		byte_index++;
	}
	
	if(ceiling)
	{
		mask = 0xFF >> (ceiling);
		dst[byte_index] &= mask;
		u8 final_byte = ((*src) >> (64-src_start-n_bits)) & 0xFF;
		dst[byte_index] ^=  (final_byte << (8-ceiling));//((*src) >> (64-(src_start+n_bits)-ceiling)) ;
		byte_index++;
	}
	return 0;
}

u8 get_bit(u8 *vect,u16 bit)
{
	return vect[bit/8] & (0x80>>(bit & 0x07)) ? 1 : 0;
}

void init_acl_payload(u8* dst,u8 *data, u8 uap, u8 length,u8 flow,u8 llid,u8 crc_flag)
/*the dst array must have the size of length plus 3 byte in case of crc requirements, 1 bytes otherwise.
	This function calculate the CRC and prepare the dst buffer for the future processing.*/
{
	u16 i,crc=0;
	u8 header = (length << 3) | (flow << 2) | llid;

	if(crc_flag)
	{
		dst[0] = header;
		for (i=0;i<length;i++)
			dst[i+1] = data[i];
		crc = compute_crc(dst,uap,8*(length+1));
		dst[0] = crc & 0xff;
		dst[1] = (crc >> 8) & 0xff;
		crc_flag = 2;
	}
	
	for (i=0;i<length;i++)
		dst[i+crc_flag] = data[length-i-1];
	dst[length+crc_flag] = header;
}
u16 fec23_codeword(u16 word)
{
	u8 i,bit_in,bit_5,state=0,add_mask = 0x15;

	for(i=0;i<10;i++)
	{
		bit_in = word & (1<<i) ? 1 : 0;
//		printf("BIT_IN %d\n",bit_in);
		bit_5 = state & (1<<4) ? 1 : 0;
		
		state = state << 1;
		if(bit_in ^ bit_5)
			state ^= add_mask;
		state &= 0x1f;
	}
//	print_byte(state);
	word &= 0x03ff;
	word ^= reverse_byte(state) << 7;
	return word;
}
u64 get_bits(u8 *src,u8 src_start,u8 n_bits)
{
	if (n_bits<1) return 0;
	u64 bits = 0;
	u8 i,byte_in,byte_start = src_start/8;
	u8 n_bytes = (src_start+n_bits-1)/8 - byte_start +1;//(src_start+n_bits)/8 - byte_start + ((src_start & 0x7) ? 1 : 0 );

//	printf("diocane n_bytes: %d byte_start: %d n_bits %d src_start %d\n",n_bytes,byte_start,n_bits,src_start);
	for(i=0;i<n_bytes;i++)
	{
		if(i==n_bytes-1 && ((src_start+n_bits) & 0x7))
			bits = bits << ((n_bits+src_start) & 0x7);	
		else
			bits = bits << 8;

		byte_in = src[byte_start+i];
//		printf("byte IN\n");
//		print_byte(byte_in);

		if(i==0 && (src_start & 0x7))
			byte_in &= 0xFF >> (src_start & 0x7);
		if(i==n_bytes-1 && ((src_start+n_bits) & 0x7))
			byte_in = byte_in >> ((8-( (src_start+n_bits) & 0x7)) & 0x7);
	
//		printf("byte IN processed\n");
//		print_byte(byte_in);
		bits ^= byte_in;
	}
	return bits;
}
void compute_fec23(u8 *dst,u8 *src,u16 n_bits)
{
	u64 buff;
	u16 word = 0;
	u8 i=0,j=0,padding = n_bits%10;

	while(i<n_bits)
	{
		if(padding)
		{
			buff = get_bits(src,i,padding);
			i+=padding;
			padding = 0;
		}
		else
		{
			buff = get_bits(src,i,10);
			i+=10;
		}
		word = buff;
		//printf("word %04x\n",word);
		word = fec23_codeword(word);
		buff = word;
		//printf("codeword %04x\n",word);
		bit_copy(dst,&buff,j,49,15);
		j+=15;
	}
/*
	u8 state = 0,bit_in,bit_5;
	u16 word = 0;
	u8 add_mask = 0x15;
	u8 dst_bit = 0,i;
	u64 tmp=0;
	u8 padding = (10-(n_bits%10))%10;

	for(i=0;i<n_bits+padding;i++)
	{
		bit_in = get_bit(src,9-(i%10)+i/10);
		//printf("nbit: %d BIT_IN %d\n",9-(i%10),bit_in);
		bit_5 = state & (1<<4) ? 1 : 0;
		state = state << 1;

		if(bit_in ^ bit_5)
			state ^= add_mask;
		state &= 0x1f; 

		word = word >> 1;
		if(bit_in)
			word ^= 0x200;

		if ((i+1)%10 == 0)
		{
//			printf("Scrittura su dst, stato:\n");
	//		print_byte(state);
			tmp = reverse_byte(state)>>3;
			bit_copy(dst,&tmp,i+5*(i/10)-9,59,5);
			tmp = word;
			bit_copy(dst,&tmp,i+5*(i/10)-4,54,10);
			printf("Scrittura su dst di codeword: %04x\n",word);
			state = 0;
			word = 0;
		}
	}
*/
}
