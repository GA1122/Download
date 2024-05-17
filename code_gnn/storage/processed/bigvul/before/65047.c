static int check_reg_arg(struct bpf_reg_state *regs, u32 regno,
			 enum reg_arg_type t)
{
	if (regno >= MAX_BPF_REG) {
		verbose("R%d is invalid\n", regno);
		return -EINVAL;
	}

	if (t == SRC_OP) {
		 
		if (regs[regno].type == NOT_INIT) {
			verbose("R%d !read_ok\n", regno);
			return -EACCES;
		}
	} else {
		 
		if (regno == BPF_REG_FP) {
			verbose("frame pointer is read only\n");
			return -EACCES;
		}
		if (t == DST_OP)
			mark_reg_unknown_value(regs, regno);
	}
	return 0;
}
