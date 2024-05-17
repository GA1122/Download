static void init_reg_state(struct reg_state *regs)
{
	int i;

	for (i = 0; i < MAX_BPF_REG; i++) {
		regs[i].type = NOT_INIT;
		regs[i].imm = 0;
		regs[i].map_ptr = NULL;
	}

	 
	regs[BPF_REG_FP].type = FRAME_PTR;

	 
	regs[BPF_REG_1].type = PTR_TO_CTX;
}
