static int check_stack_read(struct bpf_verifier_env *env,
			    struct bpf_verifier_state *state, int off, int size,
			    int value_regno)
{
	int i, slot = -off - 1, spi = slot / BPF_REG_SIZE;
	u8 *stype;

	if (state->allocated_stack <= slot) {
		verbose(env, "invalid read from stack off %d+0 size %d\n",
			off, size);
		return -EACCES;
	}
	stype = state->stack[spi].slot_type;

	if (stype[0] == STACK_SPILL) {
		if (size != BPF_REG_SIZE) {
			verbose(env, "invalid size of register spill\n");
			return -EACCES;
		}
		for (i = 1; i < BPF_REG_SIZE; i++) {
			if (stype[(slot - i) % BPF_REG_SIZE] != STACK_SPILL) {
				verbose(env, "corrupted spill memory\n");
				return -EACCES;
			}
		}

		if (value_regno >= 0) {
			 
			state->regs[value_regno] = state->stack[spi].spilled_ptr;
			mark_stack_slot_read(state, spi);
		}
		return 0;
	} else {
		for (i = 0; i < size; i++) {
			if (stype[(slot - i) % BPF_REG_SIZE] != STACK_MISC) {
				verbose(env, "invalid read from stack off %d+%d size %d\n",
					off, i, size);
				return -EACCES;
			}
		}
		if (value_regno >= 0)
			 
			mark_reg_unknown(env, state->regs, value_regno);
		return 0;
	}
}