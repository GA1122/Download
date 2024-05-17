static bool states_equal(struct bpf_verifier_env *env,
			 struct bpf_verifier_state *old,
			 struct bpf_verifier_state *cur)
{
	bool varlen_map_access = env->varlen_map_value_access;
	struct bpf_reg_state *rold, *rcur;
	int i;

	for (i = 0; i < MAX_BPF_REG; i++) {
		rold = &old->regs[i];
		rcur = &cur->regs[i];

		if (memcmp(rold, rcur, sizeof(*rold)) == 0)
			continue;

		 
		if (!varlen_map_access &&
		    memcmp(rold, rcur, offsetofend(struct bpf_reg_state, id)) == 0)
			continue;

		 
		if (rold->type == NOT_INIT ||
		    (!varlen_map_access && rold->type == UNKNOWN_VALUE &&
		     rcur->type != NOT_INIT))
			continue;

		if (rold->type == PTR_TO_PACKET && rcur->type == PTR_TO_PACKET &&
		    compare_ptrs_to_packet(rold, rcur))
			continue;

		return false;
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
