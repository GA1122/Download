INST_HANDLER (bld) {	 
	if (len < 2) {
		return;
	}
	int d = ((buf[1] & 0x01) << 4) | ((buf[0] >> 4) & 0xf);
	int b = buf[0] & 0x7;
	ESIL_A ("r%d,%d,1,<<,0xff,^,&,", d, b);			 
	ESIL_A ("%d,tf,<<,|,r%d,=,", b, d);			 
}
