static bool do_propagate_liveness(const struct bpf_verifier_state *state,
				  struct bpf_verifier_state *parent)
{
	bool writes = parent == state->parent;  
	bool touched = false;  
	int i;

	if (!parent)
		return touched;
	 
	BUILD_BUG_ON(BPF_REG_FP + 1 != MAX_BPF_REG);
	 
	for (i = 0; i < BPF_REG_FP; i++) {
		if (parent->regs[i].live & REG_LIVE_READ)
			continue;
		if (writes && (state->regs[i].live & REG_LIVE_WRITTEN))
			continue;
		if (state->regs[i].live & REG_LIVE_READ) {
			parent->regs[i].live |= REG_LIVE_READ;
			touched = true;
		}
	}
	 
	for (i = 0; i < state->allocated_stack / BPF_REG_SIZE &&
		    i < parent->allocated_stack / BPF_REG_SIZE; i++) {
		if (parent->stack[i].slot_type[0] != STACK_SPILL)
			continue;
		if (state->stack[i].slot_type[0] != STACK_SPILL)
			continue;
		if (parent->stack[i].spilled_ptr.live & REG_LIVE_READ)
			continue;
		if (writes &&
		    (state->stack[i].spilled_ptr.live & REG_LIVE_WRITTEN))
			continue;
		if (state->stack[i].spilled_ptr.live & REG_LIVE_READ) {
			parent->stack[i].spilled_ptr.live |= REG_LIVE_READ;
			touched = true;
		}
	}
	return touched;
}
