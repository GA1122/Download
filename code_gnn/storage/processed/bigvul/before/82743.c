INST_HANDLER (or) {	 
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 1) << 4);
	int r = (buf[0] & 0xf) | ((buf[1] & 2) << 3);
	ESIL_A ("r%d,r%d,|,", r, d);				 
	ESIL_A ("0,RPICK,!,zf,=,");				 
	ESIL_A ("0,RPICK,0x80,&,!,!,nf,=,");			 
	ESIL_A ("0,vf,=,");					 
	ESIL_A ("nf,sf,=,");					 
	ESIL_A ("r%d,=,", d);					 
}
