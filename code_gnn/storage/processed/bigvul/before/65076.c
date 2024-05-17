static void reg_set_min_max(struct bpf_reg_state *true_reg,
			    struct bpf_reg_state *false_reg, u64 val,
			    u8 opcode)
{
	switch (opcode) {
	case BPF_JEQ:
		 
		true_reg->max_value = true_reg->min_value = val;
		break;
	case BPF_JNE:
		 
		false_reg->max_value = false_reg->min_value = val;
		break;
	case BPF_JGT:
		 
		false_reg->min_value = 0;
		 
	case BPF_JSGT:
		 
		false_reg->max_value = val;
		true_reg->min_value = val + 1;
		break;
	case BPF_JGE:
		 
		false_reg->min_value = 0;
		 
	case BPF_JSGE:
		 
		false_reg->max_value = val - 1;
		true_reg->min_value = val;
		break;
	default:
		break;
	}

	check_reg_overflow(false_reg);
	check_reg_overflow(true_reg);
}
