static void find_good_pkt_pointers(struct bpf_verifier_state *state,
				   struct bpf_reg_state *dst_reg)
{
	struct bpf_reg_state *regs = state->regs, *reg;
	int i;

	 

	for (i = 0; i < MAX_BPF_REG; i++)
		if (regs[i].type == PTR_TO_PACKET && regs[i].id == dst_reg->id)
			 
			regs[i].range = max(regs[i].range, dst_reg->off);

	for (i = 0; i < MAX_BPF_STACK; i += BPF_REG_SIZE) {
		if (state->stack_slot_type[i] != STACK_SPILL)
			continue;
		reg = &state->spilled_regs[i / BPF_REG_SIZE];
		if (reg->type == PTR_TO_PACKET && reg->id == dst_reg->id)
			reg->range = max(reg->range, dst_reg->off);
	}
}
