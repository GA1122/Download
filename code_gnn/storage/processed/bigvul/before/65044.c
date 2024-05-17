static int check_packet_ptr_add(struct bpf_verifier_env *env,
				struct bpf_insn *insn)
{
	struct bpf_reg_state *regs = env->cur_state.regs;
	struct bpf_reg_state *dst_reg = &regs[insn->dst_reg];
	struct bpf_reg_state *src_reg = &regs[insn->src_reg];
	struct bpf_reg_state tmp_reg;
	s32 imm;

	if (BPF_SRC(insn->code) == BPF_K) {
		 
		imm = insn->imm;

add_imm:
		if (imm < 0) {
			verbose("addition of negative constant to packet pointer is not allowed\n");
			return -EACCES;
		}
		if (imm >= MAX_PACKET_OFF ||
		    imm + dst_reg->off >= MAX_PACKET_OFF) {
			verbose("constant %d is too large to add to packet pointer\n",
				imm);
			return -EACCES;
		}
		 
		dst_reg->off += imm;
	} else {
		if (src_reg->type == PTR_TO_PACKET) {
			 
			tmp_reg = *dst_reg;   
			*dst_reg = *src_reg;  
			src_reg = &tmp_reg;   
			 
		}

		if (src_reg->type == CONST_IMM) {
			 
			imm = src_reg->imm;
			goto add_imm;
		}
		 
		if (src_reg->type != UNKNOWN_VALUE) {
			verbose("cannot add '%s' to ptr_to_packet\n",
				reg_type_str[src_reg->type]);
			return -EACCES;
		}
		if (src_reg->imm < 48) {
			verbose("cannot add integer value with %lld upper zero bits to ptr_to_packet\n",
				src_reg->imm);
			return -EACCES;
		}
		 
		dst_reg->id = ++env->id_gen;

		 
		dst_reg->off = 0;
		dst_reg->range = 0;
	}
	return 0;
}
