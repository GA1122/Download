INST_HANDLER (mul) {	 
	int d = ((buf[1] << 4) & 0x10) | ((buf[0] >> 4) & 0x0f);
	int r = ((buf[1] << 3) & 0x10) | (buf[0] & 0x0f);

	ESIL_A ("r%d,r%d,*,", r, d);			 
	ESIL_A ("DUP,0xff,&,r0,=,");			 
	ESIL_A ("8,0,RPICK,>>,0xff,&,r1,=,");		 
	ESIL_A ("DUP,0x8000,&,!,!,cf,=,");		 
	ESIL_A ("DUP,!,zf,=,");				 
}
