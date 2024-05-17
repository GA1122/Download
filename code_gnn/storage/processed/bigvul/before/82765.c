INST_HANDLER (swap) {	 
	int d = ((buf[1] & 0x1) << 4) | ((buf[0] >> 4) & 0xf);
	ESIL_A ("4,r%d,>>,0x0f,&,", d);		 
	ESIL_A ("4,r%d,<<,0xf0,&,", d);		 
	ESIL_A ("|,", d);			 
	ESIL_A ("r%d,=,", d);			 
}
