static void adjust_reg_min_max_vals(struct bpf_verifier_env *env,
				    struct bpf_insn *insn)
{
	struct bpf_reg_state *regs = env->cur_state.regs, *dst_reg;
	s64 min_val = BPF_REGISTER_MIN_RANGE;
	u64 max_val = BPF_REGISTER_MAX_RANGE;
	u8 opcode = BPF_OP(insn->code);

	dst_reg = &regs[insn->dst_reg];
	if (BPF_SRC(insn->code) == BPF_X) {
		check_reg_overflow(&regs[insn->src_reg]);
		min_val = regs[insn->src_reg].min_value;
		max_val = regs[insn->src_reg].max_value;

		 
		if (regs[insn->src_reg].type != CONST_IMM &&
		    regs[insn->src_reg].type != UNKNOWN_VALUE) {
			min_val = BPF_REGISTER_MIN_RANGE;
			max_val = BPF_REGISTER_MAX_RANGE;
		}
	} else if (insn->imm < BPF_REGISTER_MAX_RANGE &&
		   (s64)insn->imm > BPF_REGISTER_MIN_RANGE) {
		min_val = max_val = insn->imm;
	}

	 
	if (min_val == BPF_REGISTER_MIN_RANGE &&
	    max_val == BPF_REGISTER_MAX_RANGE) {
		reset_reg_range_values(regs, insn->dst_reg);
		return;
	}

	 
	if (min_val == BPF_REGISTER_MIN_RANGE)
		dst_reg->min_value = BPF_REGISTER_MIN_RANGE;
	if (max_val == BPF_REGISTER_MAX_RANGE)
		dst_reg->max_value = BPF_REGISTER_MAX_RANGE;

	switch (opcode) {
	case BPF_ADD:
		if (dst_reg->min_value != BPF_REGISTER_MIN_RANGE)
			dst_reg->min_value += min_val;
		if (dst_reg->max_value != BPF_REGISTER_MAX_RANGE)
			dst_reg->max_value += max_val;
		break;
	case BPF_SUB:
		if (dst_reg->min_value != BPF_REGISTER_MIN_RANGE)
			dst_reg->min_value -= min_val;
		if (dst_reg->max_value != BPF_REGISTER_MAX_RANGE)
			dst_reg->max_value -= max_val;
		break;
	case BPF_MUL:
		if (dst_reg->min_value != BPF_REGISTER_MIN_RANGE)
			dst_reg->min_value *= min_val;
		if (dst_reg->max_value != BPF_REGISTER_MAX_RANGE)
			dst_reg->max_value *= max_val;
		break;
	case BPF_AND:
		 
		if (min_val < 0)
			dst_reg->min_value = BPF_REGISTER_MIN_RANGE;
		else
			dst_reg->min_value = 0;
		dst_reg->max_value = max_val;
		break;
	case BPF_LSH:
		 
		if (min_val > ilog2(BPF_REGISTER_MAX_RANGE))
			dst_reg->min_value = BPF_REGISTER_MIN_RANGE;
		else if (dst_reg->min_value != BPF_REGISTER_MIN_RANGE)
			dst_reg->min_value <<= min_val;

		if (max_val > ilog2(BPF_REGISTER_MAX_RANGE))
			dst_reg->max_value = BPF_REGISTER_MAX_RANGE;
		else if (dst_reg->max_value != BPF_REGISTER_MAX_RANGE)
			dst_reg->max_value <<= max_val;
		break;
	case BPF_RSH:
		 
		if (min_val < 0 || dst_reg->min_value < 0)
			dst_reg->min_value = BPF_REGISTER_MIN_RANGE;
		else
			dst_reg->min_value =
				(u64)(dst_reg->min_value) >> min_val;
		if (dst_reg->max_value != BPF_REGISTER_MAX_RANGE)
			dst_reg->max_value >>= max_val;
		break;
	default:
		reset_reg_range_values(regs, insn->dst_reg);
		break;
	}

	check_reg_overflow(dst_reg);
}
