INST_HANDLER (sbc) {	 
	int r = (buf[0] & 0x0f) | ((buf[1] & 0x2) << 3);
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 0x1) << 4);

	ESIL_A ("cf,r%d,+,r%d,-,", r, d);		 
	__generic_sub_update_flags_rr (op, d, r, 1);	 
	ESIL_A ("r%d,=,", d);				 
}
