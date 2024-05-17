static __inline__ __be32 addr_bit_set(const void *token, int fn_bit)
{
	const __be32 *addr = token;
	 
	return (__force __be32)(1 << ((~fn_bit ^ BITOP_BE32_SWIZZLE) & 0x1f)) &
	       addr[fn_bit >> 5];
}
