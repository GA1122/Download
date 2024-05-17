static void mark_stack_slot_read(struct bpf_verifier_env *env,
				 const struct bpf_verifier_state *state,
				 struct bpf_verifier_state *parent,
				 int slot, int frameno)
{
	bool writes = parent == state->parent;  

	while (parent) {
		if (parent->frame[frameno]->allocated_stack <= slot * BPF_REG_SIZE)
			 
			break;
		 
		if (writes && state->frame[frameno]->stack[slot].spilled_ptr.live & REG_LIVE_WRITTEN)
			break;
		 
		parent->frame[frameno]->stack[slot].spilled_ptr.live |= REG_LIVE_READ;
		state = parent;
		parent = state->parent;
		writes = true;
	}
}
