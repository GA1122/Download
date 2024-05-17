INST_HANDLER (lat) {	 
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 0x1) << 4);

	__generic_ld_st (op, "ram", 'z', 1, 0, 0, 0);	 
	ESIL_A ("r%d,^,", d);				 
	ESIL_A ("DUP,r%d,=,", d);			 
	__generic_ld_st (op, "ram", 'z', 1, 0, 0, 1);	 
}
