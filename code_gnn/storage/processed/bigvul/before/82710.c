INST_HANDLER (com) {	 
	int r = ((buf[0] >> 4) & 0x0f) | ((buf[1] & 1) << 4);

	ESIL_A ("r%d,0xff,-,0xff,&,r%d,=,", r, r);		 
	ESIL_A ("0,cf,=,");					 
	__generic_bitop_flags (op);				 
}
