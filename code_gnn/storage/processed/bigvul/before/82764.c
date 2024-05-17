INST_HANDLER (std) {	 
	ESIL_A ("r%d,", ((buf[1] & 1) << 4) | ((buf[0] >> 4) & 0xf));
	__generic_ld_st (
		op, "ram",
		buf[0] & 0x8 ? 'y' : 'z',	 
		0,				 
		!(buf[1] & 0x10)
			? 0			 
			: buf[0] & 0x1
				? 1		 
				: -1,		 
		!(buf[1] & 0x10)
			? (buf[1] & 0x20)	 
			| ((buf[1] & 0xc) << 1)
			| (buf[0] & 0x7)
			: 0,			 
		1);				 
}
