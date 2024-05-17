static int check_xadd(struct verifier_env *env, struct bpf_insn *insn)
{
	struct reg_state *regs = env->cur_state.regs;
	int err;

	if ((BPF_SIZE(insn->code) != BPF_W && BPF_SIZE(insn->code) != BPF_DW) ||
	    insn->imm != 0) {
		verbose("BPF_XADD uses reserved fields\n");
		return -EINVAL;
	}

	 
	err = check_reg_arg(regs, insn->src_reg, SRC_OP);
	if (err)
		return err;

	 
	err = check_reg_arg(regs, insn->dst_reg, SRC_OP);
	if (err)
		return err;

	 
	err = check_mem_access(env, insn->dst_reg, insn->off,
			       BPF_SIZE(insn->code), BPF_READ, -1);
	if (err)
		return err;

	 
	return check_mem_access(env, insn->dst_reg, insn->off,
				BPF_SIZE(insn->code), BPF_WRITE, -1);
}
