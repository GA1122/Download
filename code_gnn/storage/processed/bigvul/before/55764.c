static inline void allow_bits(struct posix_ace_state *astate, u32 mask)
{
	 
	astate->allow |= mask & ~astate->deny;
}
