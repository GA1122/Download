static int check_stack_write(struct bpf_verifier_env *env,
			     struct bpf_verifier_state *state, int off,
			     int size, int value_regno)
{
	int i, slot = -off - 1, spi = slot / BPF_REG_SIZE, err;

	err = realloc_verifier_state(state, round_up(slot + 1, BPF_REG_SIZE),
				     true);
	if (err)
		return err;
	 
	if (!env->allow_ptr_leaks &&
	    state->stack[spi].slot_type[0] == STACK_SPILL &&
	    size != BPF_REG_SIZE) {
		verbose(env, "attempt to corrupt spilled pointer on stack\n");
		return -EACCES;
	}

	if (value_regno >= 0 &&
	    is_spillable_regtype(state->regs[value_regno].type)) {

		 
		if (size != BPF_REG_SIZE) {
			verbose(env, "invalid size of register spill\n");
			return -EACCES;
		}

		 
		state->stack[spi].spilled_ptr = state->regs[value_regno];
		state->stack[spi].spilled_ptr.live |= REG_LIVE_WRITTEN;

		for (i = 0; i < BPF_REG_SIZE; i++)
			state->stack[spi].slot_type[i] = STACK_SPILL;
	} else {
		 
		state->stack[spi].spilled_ptr = (struct bpf_reg_state) {};

		for (i = 0; i < size; i++)
			state->stack[spi].slot_type[(slot - i) % BPF_REG_SIZE] =
				STACK_MISC;
	}
	return 0;
}
