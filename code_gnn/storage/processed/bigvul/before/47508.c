static inline u8 swap_bits_in_byte(u8 b)
{
#define R_SHIFT_4_MASK  0xc0  
#define R_SHIFT_2_MASK  0x28  
#define R_SHIFT_1_MASK  0x1e  
#define L_SHIFT_4_MASK  0x03  
#define L_SHIFT_2_MASK  0x14  
#define L_SHIFT_1_MASK  0x78  

	u8 n1;
	u8 n2;

	 
	 
	n1 = ((b  & R_SHIFT_4_MASK) >> 4) | (b  & ~(R_SHIFT_4_MASK >> 4));
	 
	n1 = ((n1 & R_SHIFT_2_MASK) >> 2) | (n1 & ~(R_SHIFT_2_MASK >> 2));
	 
	n1 = (n1  & R_SHIFT_1_MASK) >> 1;

	 
	 
	n2 = ((b  & L_SHIFT_4_MASK) << 4) | (b  & ~(L_SHIFT_4_MASK << 4));
	 
	n2 = ((n2 & L_SHIFT_2_MASK) << 2) | (n2 & ~(L_SHIFT_2_MASK << 2));
	 
	n2 = (n2  & L_SHIFT_1_MASK) << 1;

	return n1 | n2;
}
