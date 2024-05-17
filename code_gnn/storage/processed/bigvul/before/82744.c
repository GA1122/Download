INST_HANDLER (ori) {	 
	int d = ((buf[0] >> 4) & 0xf) + 16;
	int k = (buf[0] & 0xf) | ((buf[1] & 0xf) << 4);
	op->val = k;
	ESIL_A ("r%d,%d,|,", d, k);				 
	ESIL_A ("0,RPICK,!,zf,=,");				 
	ESIL_A ("0,RPICK,0x80,&,!,!,nf,=,");			 
	ESIL_A ("0,vf,=,");					 
	ESIL_A ("nf,sf,=,");					 
	ESIL_A ("r%d,=,", d);					 
}
