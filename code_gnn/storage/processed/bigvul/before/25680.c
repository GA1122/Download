static inline enum direction decode_direction(unsigned int insn)
{
	unsigned long tmp = (insn >> 21) & 1;

	if(!tmp)
		return load;
	else {
		if(((insn>>19)&0x3f) == 15)
			return both;
		else
			return store;
	}
}
