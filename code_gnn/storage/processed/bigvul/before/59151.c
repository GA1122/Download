static void mark_reg_unknown(struct bpf_verifier_env *env,
			     struct bpf_reg_state *regs, u32 regno)
{
	if (WARN_ON(regno >= MAX_BPF_REG)) {
		verbose(env, "mark_reg_unknown(regs, %u)\n", regno);
		 
		for (regno = 0; regno < MAX_BPF_REG; regno++)
			__mark_reg_not_init(regs + regno);
		return;
	}
	__mark_reg_unknown(regs + regno);
}