static int is_state_visited(struct bpf_verifier_env *env, int insn_idx)
{
	struct bpf_verifier_state_list *new_sl;
	struct bpf_verifier_state_list *sl;

	sl = env->explored_states[insn_idx];
	if (!sl)
		 
		return 0;

	while (sl != STATE_LIST_MARK) {
		if (states_equal(env, &sl->state, &env->cur_state))
			 
			return 1;
		sl = sl->next;
	}

	 
	new_sl = kmalloc(sizeof(struct bpf_verifier_state_list), GFP_USER);
	if (!new_sl)
		return -ENOMEM;

	 
	memcpy(&new_sl->state, &env->cur_state, sizeof(env->cur_state));
	new_sl->next = env->explored_states[insn_idx];
	env->explored_states[insn_idx] = new_sl;
	return 0;
}
