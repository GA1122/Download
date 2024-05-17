INST_HANDLER (lpm) {	 
	ut16 ins = (((ut16) buf[1]) << 8) | ((ut16) buf[0]);
	__generic_ld_st (
		op, "prog",
		'z',				 
		1,				 
		(ins & 0xfe0f) == 0x9005
			? 1			 
			: 0,			 
		0,				 
		0);				 
	ESIL_A ("r%d,=,",
		(ins == 0x95c8)
			? 0			 
			: ((buf[0] >> 4) & 0xf)	 
				| ((buf[1] & 0x1) << 4));
}
