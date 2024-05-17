INST_HANDLER (ldd) {	 
	int offset = (buf[1] & 0x20)
			| ((buf[1] & 0xc) << 1)
			| (buf[0] & 0x7);
	__generic_ld_st (
		op, "ram",
		buf[0] & 0x8 ? 'y' : 'z',	 
		0,				 
		!(buf[1] & 0x10)
			? 0			 
			: buf[0] & 0x1
				? 1		 
				: -1,		 
		!(buf[1] & 0x10) ? offset : 0,	 
		0);				 
	ESIL_A ("r%d,=,", ((buf[1] & 1) << 4) | ((buf[0] >> 4) & 0xf));
	op->cycles =
		(buf[1] & 0x10) == 0
			? (!offset ? 1 : 3)		 
			: (buf[0] & 0x3) == 0
				? 1			 
				: (buf[0] & 0x3) == 1
					? 2		 
					: 3;		 
	if (!STR_BEGINS (cpu->model, "ATxmega") && op->cycles > 1) {
		op->cycles--;
	}
}
