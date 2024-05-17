INST_HANDLER (rcall) {	 
	op->jump = (op->addr
		+ (((((buf[1] & 0xf) << 8) | buf[0]) << 1)
			| (((buf[1] & 0x8) ? ~((int) 0x1fff) : 0)))
		+ 2) & CPU_PC_MASK (cpu);
	op->fail = op->addr + op->size;
	ESIL_A ("pc,");				 
	__generic_push (op, CPU_PC_SIZE (cpu));	 
	ESIL_A ("%"PFMT64d",pc,=,", op->jump);	 
	if (!strncasecmp (cpu->model, "ATtiny", 6)) {
		op->cycles = 4;	 
	} else {
		op->cycles = cpu->pc <= 16 ? 3 : 4;
		if (!STR_BEGINS (cpu->model, "ATxmega")) {
			op->cycles--;	 
		}
	}
}
