INST_HANDLER (cp) {	 
	int r = (buf[0]        & 0x0f) | ((buf[1] << 3) & 0x10);
	int d = ((buf[0] >> 4) & 0x0f) | ((buf[1] << 4) & 0x10);
	ESIL_A ("r%d,r%d,-,", r, d);			 
	__generic_sub_update_flags_rr (op, d, r, 0);	 
}
