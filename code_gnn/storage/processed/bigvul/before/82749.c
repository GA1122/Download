INST_HANDLER (ret) {	 
	op->eob = true;
	__generic_pop (op, CPU_PC_SIZE (cpu));
	ESIL_A ("pc,=,");	 
	if (CPU_PC_SIZE (cpu) > 2) {	 
		op->cycles++;	 
	}
}
