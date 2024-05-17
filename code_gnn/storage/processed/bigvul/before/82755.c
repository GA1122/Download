INST_HANDLER (sub) {	 
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 1) << 4);
	int r = (buf[0] & 0xf) | ((buf[1] & 2) << 3);

	ESIL_A ("r%d,r%d,-,", r, d);			 
	__generic_sub_update_flags_rr (op, d, r, 0);	 
	ESIL_A ("r%d,=,", d);				 
}
