INST_HANDLER (cpi) {  
	int d = ((buf[0] >> 4) & 0xf) + 16;
	int k = (buf[0] & 0xf) | ((buf[1] & 0xf) << 4);
	ESIL_A ("%d,r%d,-,", k, d);			 
	__generic_sub_update_flags_rk (op, d, k, 0);	 
}
