static int check_map_access(struct bpf_verifier_env *env, u32 regno,
			    int off, int size, bool zero_size_allowed)
{
	struct bpf_verifier_state *vstate = env->cur_state;
	struct bpf_func_state *state = vstate->frame[vstate->curframe];
	struct bpf_reg_state *reg = &state->regs[regno];
	int err;

	 
	if (env->log.level)
		print_verifier_state(env, state);
	 
	if (reg->smin_value < 0) {
		verbose(env, "R%d min value is negative, either use unsigned index or do a if (index >=0) check.\n",
			regno);
		return -EACCES;
	}
	err = __check_map_access(env, regno, reg->smin_value + off, size,
				 zero_size_allowed);
	if (err) {
		verbose(env, "R%d min value is outside of the array range\n",
			regno);
		return err;
	}

	 
	if (reg->umax_value >= BPF_MAX_VAR_OFF) {
		verbose(env, "R%d unbounded memory access, make sure to bounds check any array access into a map\n",
			regno);
		return -EACCES;
	}
	err = __check_map_access(env, regno, reg->umax_value + off, size,
				 zero_size_allowed);
	if (err)
		verbose(env, "R%d max value is outside of the array range\n",
			regno);
	return err;
}
