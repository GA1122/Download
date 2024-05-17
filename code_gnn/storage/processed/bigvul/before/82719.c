INST_HANDLER (fmul) {	 
	int d = ((buf[0] >> 4) & 0x7) + 16;
	int r = (buf[0] & 0x7) + 16;

	ESIL_A ("1,r%d,r%d,*,<<,", r, d);		 
	ESIL_A ("0xffff,&,");				 
	ESIL_A ("DUP,0xff,&,r0,=,");			 
	ESIL_A ("8,0,RPICK,>>,0xff,&,r1,=,");		 
	ESIL_A ("DUP,0x8000,&,!,!,cf,=,");		 
	ESIL_A ("DUP,!,zf,=,");				 
}
