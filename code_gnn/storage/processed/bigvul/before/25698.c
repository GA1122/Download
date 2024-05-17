static inline enum direction decode_direction(unsigned int insn)
{
	unsigned long tmp = (insn >> 21) & 1;

	if (!tmp)
		return load;
	else {
		switch ((insn>>19)&0xf) {
		case 15:  
			return both;
		default:
			return store;
		}
	}
}