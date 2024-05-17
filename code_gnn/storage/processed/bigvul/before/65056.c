static int evaluate_reg_alu(struct bpf_verifier_env *env, struct bpf_insn *insn)
{
	struct bpf_reg_state *regs = env->cur_state.regs;
	struct bpf_reg_state *dst_reg = &regs[insn->dst_reg];
	u8 opcode = BPF_OP(insn->code);
	s64 imm_log2;

	 

	if (BPF_SRC(insn->code) == BPF_X) {
		struct bpf_reg_state *src_reg = &regs[insn->src_reg];

		if (src_reg->type == UNKNOWN_VALUE && src_reg->imm > 0 &&
		    dst_reg->imm && opcode == BPF_ADD) {
			 
			dst_reg->imm = min(dst_reg->imm, src_reg->imm);
			dst_reg->imm--;
			return 0;
		}
		if (src_reg->type == CONST_IMM && src_reg->imm > 0 &&
		    dst_reg->imm && opcode == BPF_ADD) {
			 
			imm_log2 = __ilog2_u64((long long)src_reg->imm);
			dst_reg->imm = min(dst_reg->imm, 63 - imm_log2);
			dst_reg->imm--;
			return 0;
		}
		 
		dst_reg->imm = 0;
		return 0;
	}

	 
	imm_log2 = __ilog2_u64((long long)insn->imm);

	if (dst_reg->imm && opcode == BPF_LSH) {
		 
		dst_reg->imm -= insn->imm;
	} else if (dst_reg->imm && opcode == BPF_MUL) {
		 
		dst_reg->imm -= imm_log2 + 1;
	} else if (opcode == BPF_AND) {
		 
		dst_reg->imm = 63 - imm_log2;
	} else if (dst_reg->imm && opcode == BPF_ADD) {
		 
		dst_reg->imm = min(dst_reg->imm, 63 - imm_log2);
		dst_reg->imm--;
	} else if (opcode == BPF_RSH) {
		 
		dst_reg->imm += insn->imm;
		if (unlikely(dst_reg->imm > 64))
			 
			dst_reg->imm = 64;
	} else {
		 
		dst_reg->imm = 0;
	}

	if (dst_reg->imm < 0) {
		 
		dst_reg->imm = 0;
	}
	return 0;
}
