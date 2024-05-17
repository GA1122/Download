static void find_good_pkt_pointers(struct bpf_verifier_state *vstate,
				   struct bpf_reg_state *dst_reg,
				   enum bpf_reg_type type,
				   bool range_right_open)
{
	struct bpf_func_state *state = vstate->frame[vstate->curframe];
	struct bpf_reg_state *regs = state->regs, *reg;
	u16 new_range;
	int i, j;

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

	for (j = 0; j <= vstate->curframe; j++) {
		state = vstate->frame[j];
		bpf_for_each_spilled_reg(i, state, reg) {
			if (!reg)
				continue;
			if (reg->type == type && reg->id == dst_reg->id)
				reg->range = max(reg->range, new_range);
		}
	}
}
