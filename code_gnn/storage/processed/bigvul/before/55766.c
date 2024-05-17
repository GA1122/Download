static inline void deny_bits(struct posix_ace_state *astate, u32 mask)
{
	 
	astate->deny |= mask & ~astate->allow;
}
