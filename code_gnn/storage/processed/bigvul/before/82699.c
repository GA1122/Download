INST_HANDLER (and) {	 
	if (len < 2) {
		return;
	}
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 1) << 4);
	int r = (buf[0] & 0xf) | ((buf[1] & 2) << 3);
	ESIL_A ("r%d,r%d,&,", r, d);				 
	__generic_bitop_flags (op);				 
	ESIL_A ("r%d,=,", d);					 
}
