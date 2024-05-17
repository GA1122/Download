INST_HANDLER (dec) {	 
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 0x1) << 4);
	ESIL_A ("-1,r%d,+,", d);				 
	ESIL_A ("0,RPICK,0x7f,==,vf,=,");			 
	ESIL_A ("0,RPICK,0x80,&,!,!,nf,=,");			 
	ESIL_A ("0,RPICK,!,zf,=,");				 
	ESIL_A ("vf,nf,^,sf,=,");				 
	ESIL_A ("r%d,=,", d);					 
}
