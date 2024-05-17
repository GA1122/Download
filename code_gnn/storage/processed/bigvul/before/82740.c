INST_HANDLER (muls) {	 
	int d = (buf[0] >> 4 & 0x0f) + 16;
	int r = (buf[0] & 0x0f) + 16;

	ESIL_A ("r%d,DUP,0x80,&,?{,0xffff00,|,},", r);	 
	ESIL_A ("r%d,DUP,0x80,&,?{,0xffff00,|,},", d);	 
	ESIL_A ("*,");					 
	ESIL_A ("DUP,0xff,&,r0,=,");			 
	ESIL_A ("8,0,RPICK,>>,0xff,&,r1,=,");		 
	ESIL_A ("DUP,0x8000,&,!,!,cf,=,");		 
	ESIL_A ("DUP,!,zf,=,");				 
}
