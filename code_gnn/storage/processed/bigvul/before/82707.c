INST_HANDLER (bst) {	 
	if (len < 2) {
		return;
	}
	ESIL_A ("r%d,%d,1,<<,&,!,!,tf,=,",			 
		((buf[1] & 1) << 4) | ((buf[0] >> 4) & 0xf),	 
		buf[0] & 0x7);					 
}
