INST_HANDLER (adiw) {	 
	int d = ((buf[0] & 0x30) >> 3) + 24;
	int k = (buf[0] & 0xf) | ((buf[0] >> 2) & 0x30);
	op->val = k;
	ESIL_A ("r%d:r%d,%d,+,", d + 1, d, k);			 
	ESIL_A ("r%d,0x80,&,!,"					 
		"0,RPICK,0x8000,&,!,!,"
		"&,vf,=,", d + 1);
	ESIL_A ("0,RPICK,0x8000,&,!,!,nf,=,");			 
	ESIL_A ("0,RPICK,!,zf,=,");				 
	ESIL_A ("r%d,0x80,&,!,!,"				 
		"0,RPICK,0x8000,&,!,"
		"&,cf,=,", d + 1);
	ESIL_A ("vf,nf,^,sf,=,");				 
	ESIL_A ("r%d:r%d,=,", d + 1, d);			 
}
