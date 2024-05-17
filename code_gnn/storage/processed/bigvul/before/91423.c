static int check_stack_write(struct bpf_verifier_env *env,
			     struct bpf_func_state *state,  
			     int off, int size, int value_regno, int insn_idx)
{
	struct bpf_func_state *cur;  
	int i, slot = -off - 1, spi = slot / BPF_REG_SIZE, err;
	enum bpf_reg_type type;

	err = realloc_func_state(state, round_up(slot + 1, BPF_REG_SIZE),
				 state->acquired_refs, true);
	if (err)
		return err;
	 
	if (!env->allow_ptr_leaks &&
	    state->stack[spi].slot_type[0] == STACK_SPILL &&
	    size != BPF_REG_SIZE) {
		verbose(env, "attempt to corrupt spilled pointer on stack\n");
		return -EACCES;
	}

	cur = env->cur_state->frame[env->cur_state->curframe];
	if (value_regno >= 0 &&
	    is_spillable_regtype((type = cur->regs[value_regno].type))) {

		 
		if (size != BPF_REG_SIZE) {
			verbose(env, "invalid size of register spill\n");
			return -EACCES;
		}

		if (state != cur && type == PTR_TO_STACK) {
			verbose(env, "cannot spill pointers to stack into stack frame of the caller\n");
			return -EINVAL;
		}

		 
		state->stack[spi].spilled_ptr = cur->regs[value_regno];
		state->stack[spi].spilled_ptr.live |= REG_LIVE_WRITTEN;

		for (i = 0; i < BPF_REG_SIZE; i++) {
			if (state->stack[spi].slot_type[i] == STACK_MISC &&
			    !env->allow_ptr_leaks) {
				int *poff = &env->insn_aux_data[insn_idx].sanitize_stack_off;
				int soff = (-spi - 1) * BPF_REG_SIZE;

				 
				if (*poff && *poff != soff) {
					 
					verbose(env,
						"insn %d cannot access two stack slots fp%d and fp%d",
						insn_idx, *poff, soff);
					return -EINVAL;
				}
				*poff = soff;
			}
			state->stack[spi].slot_type[i] = STACK_SPILL;
		}
	} else {
		u8 type = STACK_MISC;

		 
		state->stack[spi].spilled_ptr.type = NOT_INIT;
		 
		if (state->stack[spi].slot_type[0] == STACK_SPILL)
			for (i = 0; i < BPF_REG_SIZE; i++)
				state->stack[spi].slot_type[i] = STACK_MISC;

		 
		if (size == BPF_REG_SIZE)
			state->stack[spi].spilled_ptr.live |= REG_LIVE_WRITTEN;

		 
		if (value_regno >= 0 &&
		    register_is_null(&cur->regs[value_regno]))
			type = STACK_ZERO;

		 
		for (i = 0; i < size; i++)
			state->stack[spi].slot_type[(slot - i) % BPF_REG_SIZE] =
				type;
	}
	return 0;
}
