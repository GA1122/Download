static int check_xadd(struct bpf_verifier_env *env, int insn_idx, struct bpf_insn *insn)
{
	int err;

	if ((BPF_SIZE(insn->code) != BPF_W && BPF_SIZE(insn->code) != BPF_DW) ||
	    insn->imm != 0) {
		verbose(env, "BPF_XADD uses reserved fields\n");
		return -EINVAL;
	}

	 
	err = check_reg_arg(env, insn->src_reg, SRC_OP);
	if (err)
		return err;

	 
	err = check_reg_arg(env, insn->dst_reg, SRC_OP);
	if (err)
		return err;

	if (is_pointer_value(env, insn->src_reg)) {
		verbose(env, "R%d leaks addr into mem\n", insn->src_reg);
		return -EACCES;
	}

	 
	err = check_mem_access(env, insn_idx, insn->dst_reg, insn->off,
			       BPF_SIZE(insn->code), BPF_READ, -1);
	if (err)
		return err;

	 
	return check_mem_access(env, insn_idx, insn->dst_reg, insn->off,
				BPF_SIZE(insn->code), BPF_WRITE, -1);
}
