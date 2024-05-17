static int check_ld_abs(struct bpf_verifier_env *env, struct bpf_insn *insn)
{
	struct bpf_reg_state *regs = env->cur_state.regs;
	u8 mode = BPF_MODE(insn->code);
	struct bpf_reg_state *reg;
	int i, err;

	if (!may_access_skb(env->prog->type)) {
		verbose("BPF_LD_[ABS|IND] instructions not allowed for this program type\n");
		return -EINVAL;
	}

	if (insn->dst_reg != BPF_REG_0 || insn->off != 0 ||
	    BPF_SIZE(insn->code) == BPF_DW ||
	    (mode == BPF_ABS && insn->src_reg != BPF_REG_0)) {
		verbose("BPF_LD_[ABS|IND] uses reserved fields\n");
		return -EINVAL;
	}

	 
	err = check_reg_arg(regs, BPF_REG_6, SRC_OP);
	if (err)
		return err;

	if (regs[BPF_REG_6].type != PTR_TO_CTX) {
		verbose("at the time of BPF_LD_ABS|IND R6 != pointer to skb\n");
		return -EINVAL;
	}

	if (mode == BPF_IND) {
		 
		err = check_reg_arg(regs, insn->src_reg, SRC_OP);
		if (err)
			return err;
	}

	 
	for (i = 0; i < CALLER_SAVED_REGS; i++) {
		reg = regs + caller_saved[i];
		reg->type = NOT_INIT;
		reg->imm = 0;
	}

	 
	regs[BPF_REG_0].type = UNKNOWN_VALUE;
	return 0;
}