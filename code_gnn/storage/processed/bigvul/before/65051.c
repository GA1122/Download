static int check_stack_write(struct bpf_verifier_state *state, int off,
			     int size, int value_regno)
{
	int i;
	 

	if (value_regno >= 0 &&
	    is_spillable_regtype(state->regs[value_regno].type)) {

		 
		if (size != BPF_REG_SIZE) {
			verbose("invalid size of register spill\n");
			return -EACCES;
		}

		 
		state->spilled_regs[(MAX_BPF_STACK + off) / BPF_REG_SIZE] =
			state->regs[value_regno];

		for (i = 0; i < BPF_REG_SIZE; i++)
			state->stack_slot_type[MAX_BPF_STACK + off + i] = STACK_SPILL;
	} else {
		 
		state->spilled_regs[(MAX_BPF_STACK + off) / BPF_REG_SIZE] =
			(struct bpf_reg_state) {};

		for (i = 0; i < size; i++)
			state->stack_slot_type[MAX_BPF_STACK + off + i] = STACK_MISC;
	}
	return 0;
}
