static void find_good_pkt_pointers(struct bpf_verifier_state *state,
				   struct bpf_reg_state *dst_reg,
				   enum bpf_reg_type type,
				   bool range_right_open)
{
	struct bpf_reg_state *regs = state->regs, *reg;
	u16 new_range;
	int i;

	if (dst_reg->off < 0 ||
	    (dst_reg->off == 0 && range_right_open))
		 
		return;

	if (dst_reg->umax_value > MAX_PACKET_OFF ||
	    dst_reg->umax_value + dst_reg->off > MAX_PACKET_OFF)
		 
		return;

	new_range = dst_reg->off;
	if (range_right_open)
		new_range--;

	 

	 
	for (i = 0; i < MAX_BPF_REG; i++)
		if (regs[i].type == type && regs[i].id == dst_reg->id)
			 
			regs[i].range = max(regs[i].range, new_range);

	for (i = 0; i < state->allocated_stack / BPF_REG_SIZE; i++) {
		if (state->stack[i].slot_type[0] != STACK_SPILL)
			continue;
		reg = &state->stack[i].spilled_ptr;
		if (reg->type == type && reg->id == dst_reg->id)
			reg->range = max(reg->range, new_range);
	}
}
