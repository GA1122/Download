INST_HANDLER (andi) {	 
	if (len < 2) {
		return;
	}
	int d = ((buf[0] >> 4) & 0xf) + 16;
	int k = ((buf[1] & 0x0f) << 4) | (buf[0] & 0x0f);
	op->val = k;
	ESIL_A ("%d,r%d,&,", k, d);				 
	__generic_bitop_flags (op);				 
	ESIL_A ("r%d,=,", d);					 
}