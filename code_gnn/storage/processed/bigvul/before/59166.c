static void reg_set_min_max_inv(struct bpf_reg_state *true_reg,
				struct bpf_reg_state *false_reg, u64 val,
				u8 opcode)
{
	if (__is_pointer_value(false, false_reg))
		return;

	switch (opcode) {
	case BPF_JEQ:
		 
		__mark_reg_known(true_reg, val);
		break;
	case BPF_JNE:
		 
		__mark_reg_known(false_reg, val);
		break;
	case BPF_JGT:
		true_reg->umax_value = min(true_reg->umax_value, val - 1);
		false_reg->umin_value = max(false_reg->umin_value, val);
		break;
	case BPF_JSGT:
		true_reg->smax_value = min_t(s64, true_reg->smax_value, val - 1);
		false_reg->smin_value = max_t(s64, false_reg->smin_value, val);
		break;
	case BPF_JLT:
		true_reg->umin_value = max(true_reg->umin_value, val + 1);
		false_reg->umax_value = min(false_reg->umax_value, val);
		break;
	case BPF_JSLT:
		true_reg->smin_value = max_t(s64, true_reg->smin_value, val + 1);
		false_reg->smax_value = min_t(s64, false_reg->smax_value, val);
		break;
	case BPF_JGE:
		true_reg->umax_value = min(true_reg->umax_value, val);
		false_reg->umin_value = max(false_reg->umin_value, val + 1);
		break;
	case BPF_JSGE:
		true_reg->smax_value = min_t(s64, true_reg->smax_value, val);
		false_reg->smin_value = max_t(s64, false_reg->smin_value, val + 1);
		break;
	case BPF_JLE:
		true_reg->umin_value = max(true_reg->umin_value, val);
		false_reg->umax_value = min(false_reg->umax_value, val - 1);
		break;
	case BPF_JSLE:
		true_reg->smin_value = max_t(s64, true_reg->smin_value, val);
		false_reg->smax_value = min_t(s64, false_reg->smax_value, val - 1);
		break;
	default:
		break;
	}

	__reg_deduce_bounds(false_reg);
	__reg_deduce_bounds(true_reg);
	 
	__reg_bound_offset(false_reg);
	__reg_bound_offset(true_reg);
	 
	__update_reg_bounds(false_reg);
	__update_reg_bounds(true_reg);
}
