static void cipso_v4_bitmap_setbit(unsigned char *bitmap,
				   u32 bit,
				   u8 state)
{
	u32 byte_spot;
	u8 bitmask;

	 
	byte_spot = bit / 8;
	bitmask = 0x80 >> (bit % 8);
	if (state)
		bitmap[byte_spot] |= bitmask;
	else
		bitmap[byte_spot] &= ~bitmask;
}