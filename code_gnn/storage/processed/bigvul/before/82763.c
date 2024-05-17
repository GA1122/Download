INST_HANDLER (st) {	 
	ESIL_A ("r%d,", ((buf[1] & 1) << 4) | ((buf[0] >> 4) & 0xf));
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
		1);				 
}
