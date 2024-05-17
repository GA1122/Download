INST_HANDLER (pop) {	 
	int d = ((buf[1] & 0x1) << 4) | ((buf[0] >> 4) & 0xf);
	__generic_pop (op, 1);
	ESIL_A ("r%d,=,", d);	 

}
