static inline bool __bss_tim_get(u8 *tim, u16 id)
{
	 
	return tim[id / 8] & (1 << (id % 8));
}
