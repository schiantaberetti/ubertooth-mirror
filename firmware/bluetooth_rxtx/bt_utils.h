#ifndef _BT_UTILS_H_
#define _BT_UTILS_H_ 1
#include"types.h"

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
