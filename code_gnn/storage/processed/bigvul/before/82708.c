INST_HANDLER (call) {	 
	if (len < 4) {
		return;
	}
	op->jump = (buf[2] << 1)
		 | (buf[3] << 9)
		 | (buf[1] & 0x01) << 23
		 | (buf[0] & 0x01) << 17
		 | (buf[0] & 0xf0) << 14;
	op->fail = op->addr + op->size;
	op->cycles = cpu->pc <= 16 ? 3 : 4;
	if (!STR_BEGINS (cpu->model, "ATxmega")) {
		op->cycles--;	 
	}
	ESIL_A ("pc,");				 
	__generic_push (op, CPU_PC_SIZE (cpu));	 
	ESIL_A ("%"PFMT64d",pc,=,", op->jump);	 
}
