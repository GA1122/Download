static void mark_stack_slot_read(const struct bpf_verifier_state *state, int slot)
{
	struct bpf_verifier_state *parent = state->parent;

	while (parent) {
		 
		if (state->stack[slot].spilled_ptr.live & REG_LIVE_WRITTEN)
			break;
		 
		parent->stack[slot].spilled_ptr.live |= REG_LIVE_READ;
		state = parent;
		parent = state->parent;
	}
}
