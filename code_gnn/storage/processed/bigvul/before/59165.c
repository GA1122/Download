static void reg_set_min_max(struct bpf_reg_state *true_reg,
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
		false_reg->umax_value = min(false_reg->umax_value, val);
		true_reg->umin_value = max(true_reg->umin_value, val + 1);
		break;
	case BPF_JSGT:
		false_reg->smax_value = min_t(s64, false_reg->smax_value, val);
		true_reg->smin_value = max_t(s64, true_reg->smin_value, val + 1);
		break;
	case BPF_JLT:
		false_reg->umin_value = max(false_reg->umin_value, val);
		true_reg->umax_value = min(true_reg->umax_value, val - 1);
		break;
	case BPF_JSLT:
		false_reg->smin_value = max_t(s64, false_reg->smin_value, val);
		true_reg->smax_value = min_t(s64, true_reg->smax_value, val - 1);
		break;
	case BPF_JGE:
		false_reg->umax_value = min(false_reg->umax_value, val - 1);
		true_reg->umin_value = max(true_reg->umin_value, val);
		break;
	case BPF_JSGE:
		false_reg->smax_value = min_t(s64, false_reg->smax_value, val - 1);
		true_reg->smin_value = max_t(s64, true_reg->smin_value, val);
		break;
	case BPF_JLE:
		false_reg->umin_value = max(false_reg->umin_value, val + 1);
		true_reg->umax_value = min(true_reg->umax_value, val);
		break;
	case BPF_JSLE:
		false_reg->smin_value = max_t(s64, false_reg->smin_value, val + 1);
		true_reg->smax_value = min_t(s64, true_reg->smax_value, val);
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
