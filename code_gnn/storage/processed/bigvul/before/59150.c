static void mark_reg_read(const struct bpf_verifier_state *state, u32 regno)
{
	struct bpf_verifier_state *parent = state->parent;

	if (regno == BPF_REG_FP)
		 
		return;

	while (parent) {
		 
		if (state->regs[regno].live & REG_LIVE_WRITTEN)
			break;
		 
		parent->regs[regno].live |= REG_LIVE_READ;
		state = parent;
		parent = state->parent;
	}
}
