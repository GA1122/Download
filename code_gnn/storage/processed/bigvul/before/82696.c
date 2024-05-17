INST_HANDLER (adc) {	 
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 1) << 4);
	int r = (buf[0] & 0xf) | ((buf[1] & 2) << 3);
	ESIL_A ("r%d,cf,+,r%d,+,", r, d);		 
	__generic_add_update_flags_rr(op, d, r);	 
	ESIL_A ("r%d,=,", d);				 
}
