INST_HANDLER (subi) {	 
	int d = ((buf[0] >> 4) & 0xf) + 16;
	int k = ((buf[1] & 0xf) << 4) | (buf[0] & 0xf);
	op->val = k;

	ESIL_A ("%d,r%d,-,", k, d);			 
	__generic_sub_update_flags_rk (op, d, k, 1);	 
	ESIL_A ("r%d,=,", d);				 
}
