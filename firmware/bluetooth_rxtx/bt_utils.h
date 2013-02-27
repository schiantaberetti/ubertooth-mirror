#ifndef _BT_UTILS_H_
#define _BT_UTILS_H_ 1
#include"types.h"

static const u64 sw_matrix[] = {
	0xfe000002a0d1c014, 0x01000003f0b9201f, 0x008000033ae40edb, 0x004000035fca99b9,
	0x002000036d5dd208, 0x00100001b6aee904, 0x00080000db577482, 0x000400006dabba41,
	0x00020002f46d43f4, 0x000100017a36a1fa, 0x00008000bd1b50fd, 0x000040029c3536aa,
	0x000020014e1a9b55, 0x0000100265b5d37e, 0x0000080132dae9bf, 0x000004025bd5ea0b,
	0x00000203ef526bd1, 0x000001033511ab3c, 0x000000819a88d59e, 0x00000040cd446acf,
	0x00000022a41aabb3, 0x0000001390b5cb0d, 0x0000000b0ae27b52, 0x0000000585713da9};

void gen_access_code(u8 *access_code,u64 address);
/*Generate the access code. The access_code array is required to have a size of at least 9 bytes. The access code computed is in the 'air' order (i.e. LSB to MSB).*/

u8 reverse_byte(u8 byte);

u16 compute_crc(u8 *data,u16 state,u8 nbits);

u8 compute_hec(u16 header,u8 state);

void compute_fec13(u8 *dest,u8 *src,u8 bits);

u8 whitening_bit(u8 *state);

u8 bit_copy(u8 *dst,u64 *src,u16 dst_start,u16 src_start,u16 n_bits);
/*Copy nbits from variable src into array dst. Returns 0 if everything is ok, 1 otherwise*/

u8 get_bit(u8 *vect,u16 bit);

void compute_fec23(u8 *dst,u8 *src,u16 n_bits);

u8 compute_whitening(u8 *dst,u8 *src,u16 n_bits,u8 state);
/*It returns the state of the LFSR*/

void set_bit(u8 *vect,u16 bit,u8 value);

void generate_header(u8 *dst,u8 uap,u8 seqn,u8 arqn,u8 flow,u8 type,u8 lt_addr);
/*Generate the header of a BT packet. The dst array must have a size of 3 bytes at least.*/

void init_acl_payload(u8* dst,u8 *data, u8 uap, u8 length,u8 flow,u8 llid,u8 crc_flag);
/*the dst array must have the size of length plus 3 byte in case of crc requirements, 1 bytes otherwise.
	This function calculate the CRC if the crc_flag is not zero and prepare the dst buffer for the future processing.*/

u16 fec23_codeword(u16 word);

u64 get_bits(u8 *src,u8 src_start,u8 n_bits);

u16 merge_pkt_parts(u8 *pkt,u8 *access_code,u8 *header, u8* payload,u16 load_bits);
#endif
