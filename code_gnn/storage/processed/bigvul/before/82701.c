INST_HANDLER (asr) {	 
	if (len < 2) {
		return;
	}
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 1) << 4);
	ESIL_A ("1,r%d,>>,r%d,0x80,&,|,", d, d);		 
	ESIL_A ("r%d,0x1,&,!,!,cf,=,", d);			 
	ESIL_A ("0,RPICK,!,zf,=,");				 
	ESIL_A ("0,RPICK,0x80,&,!,!,nf,=,");			 
	ESIL_A ("nf,cf,^,vf,=,");				 
	ESIL_A ("nf,vf,^,sf,=,");				 
	ESIL_A ("r%d,=,", d);					 
}
