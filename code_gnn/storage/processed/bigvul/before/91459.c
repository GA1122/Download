static int push_insn(int t, int w, int e, struct bpf_verifier_env *env)
{
	if (e == FALLTHROUGH && insn_state[t] >= (DISCOVERED | FALLTHROUGH))
		return 0;

	if (e == BRANCH && insn_state[t] >= (DISCOVERED | BRANCH))
		return 0;

	if (w < 0 || w >= env->prog->len) {
		verbose_linfo(env, t, "%d: ", t);
		verbose(env, "jump out of range from insn %d to %d\n", t, w);
		return -EINVAL;
	}

	if (e == BRANCH)
		 
		env->explored_states[w] = STATE_LIST_MARK;

	if (insn_state[w] == 0) {
		 
		insn_state[t] = DISCOVERED | e;
		insn_state[w] = DISCOVERED;
		if (cur_stack >= env->prog->len)
			return -E2BIG;
		insn_stack[cur_stack++] = w;
		return 1;
	} else if ((insn_state[w] & 0xF0) == DISCOVERED) {
		verbose_linfo(env, t, "%d: ", t);
		verbose_linfo(env, w, "%d: ", w);
		verbose(env, "back-edge from insn %d to %d\n", t, w);
		return -EINVAL;
	} else if (insn_state[w] == EXPLORED) {
		 
		insn_state[t] = DISCOVERED | e;
	} else {
		verbose(env, "insn state internal bug\n");
		return -EFAULT;
	}
	return 0;
}