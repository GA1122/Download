static int check_map_access_adj(struct bpf_verifier_env *env, u32 regno,
				int off, int size)
{
	struct bpf_verifier_state *state = &env->cur_state;
	struct bpf_reg_state *reg = &state->regs[regno];
	int err;

	 
	if (log_level)
		print_verifier_state(state);
	env->varlen_map_value_access = true;
	 
	if (reg->min_value < 0) {
		verbose("R%d min value is negative, either use unsigned index or do a if (index >=0) check.\n",
			regno);
		return -EACCES;
	}
	err = check_map_access(env, regno, reg->min_value + off, size);
	if (err) {
		verbose("R%d min value is outside of the array range\n",
			regno);
		return err;
	}

	 
	if (reg->max_value == BPF_REGISTER_MAX_RANGE) {
		verbose("R%d unbounded memory access, make sure to bounds check any array access into a map\n",
			regno);
		return -EACCES;
	}
	return check_map_access(env, regno, reg->max_value + off, size);
}
