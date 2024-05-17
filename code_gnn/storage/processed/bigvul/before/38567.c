static inline void __bss_tim_set(u8 *tim, u16 id)
{
	 
	tim[id / 8] |= (1 << (id % 8));
}
