INST_HANDLER (push) {	 
	int r = ((buf[1] & 0x1) << 4) | ((buf[0] >> 4) & 0xf);
	ESIL_A ("r%d,", r);	 
	__generic_push (op, 1);	 
	op->cycles = !STR_BEGINS (cpu->model, "ATxmega")
			? 1	 
			: 2;
}
