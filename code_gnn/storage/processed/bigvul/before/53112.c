static bool states_equal(struct verifier_state *old, struct verifier_state *cur)
{
	int i;

	for (i = 0; i < MAX_BPF_REG; i++) {
		if (memcmp(&old->regs[i], &cur->regs[i],
			   sizeof(old->regs[0])) != 0) {
			if (old->regs[i].type == NOT_INIT ||
			    (old->regs[i].type == UNKNOWN_VALUE &&
			     cur->regs[i].type != NOT_INIT))
				continue;
			return false;
		}
	}

	for (i = 0; i < MAX_BPF_STACK; i++) {
		if (old->stack_slot_type[i] == STACK_INVALID)
			continue;
		if (old->stack_slot_type[i] != cur->stack_slot_type[i])
			 
			return false;
		if (i % BPF_REG_SIZE)
			continue;
		if (memcmp(&old->spilled_regs[i / BPF_REG_SIZE],
			   &cur->spilled_regs[i / BPF_REG_SIZE],
			   sizeof(old->spilled_regs[0])))
			 
			return false;
		else
			continue;
	}
	return true;
}
