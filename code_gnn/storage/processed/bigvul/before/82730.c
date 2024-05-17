INST_HANDLER (ld) {	 
	__generic_ld_st (
		op, "ram",
		'x',				 
		0,				 
		(buf[0] & 0xf) == 0xe
			? -1			 
			: (buf[0] & 0xf) == 0xd
				? 1		 
				: 0,		 
		0,				 
		0);				 
	ESIL_A ("r%d,=,", ((buf[1] & 1) << 4) | ((buf[0] >> 4) & 0xf));
	op->cycles = (buf[0] & 0x3) == 0
			? 2			 
			: (buf[0] & 0x3) == 1
				? 2		 
				: 3;		 
	if (!STR_BEGINS (cpu->model, "ATxmega") && op->cycles > 1) {
		op->cycles--;
	}
}
