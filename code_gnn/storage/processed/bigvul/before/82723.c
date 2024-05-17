INST_HANDLER (icall) {	 
	ESIL_A ("pc,");				 
	__generic_push (op, CPU_PC_SIZE (cpu));	 
	INST_CALL (ijmp);
	if (!STR_BEGINS (cpu->model, "ATxmega")) {
		op->cycles--;
	}
}
