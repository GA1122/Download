INST_HANDLER (ror) {	 
	int d = ((buf[0] >> 4) & 0x0f) | ((buf[1] << 4) & 0x10);
	ESIL_A ("1,r%d,>>,7,cf,<<,|,", d);		 
	ESIL_A ("r%d,1,&,cf,=,", d);			 
	ESIL_A ("0,RPICK,!,zf,=,");			 
	ESIL_A ("0,RPICK,0x80,&,!,!,nf,=,");		 
	ESIL_A ("nf,cf,^,vf,=,");			 
	ESIL_A ("vf,nf,^,sf,=,");			 
	ESIL_A ("r%d,=,", d);				 
}
