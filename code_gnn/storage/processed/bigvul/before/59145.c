static int is_state_visited(struct bpf_verifier_env *env, int insn_idx)
{
	struct bpf_verifier_state_list *new_sl;
	struct bpf_verifier_state_list *sl;
	struct bpf_verifier_state *cur = env->cur_state;
	int i, err;

	sl = env->explored_states[insn_idx];
	if (!sl)
		 
		return 0;

	while (sl != STATE_LIST_MARK) {
		if (states_equal(env, &sl->state, cur)) {
			 
			propagate_liveness(&sl->state, cur);
			return 1;
		}
		sl = sl->next;
	}

	 
	new_sl = kzalloc(sizeof(struct bpf_verifier_state_list), GFP_KERNEL);
	if (!new_sl)
		return -ENOMEM;

	 
	err = copy_verifier_state(&new_sl->state, cur);
	if (err) {
		free_verifier_state(&new_sl->state, false);
		kfree(new_sl);
		return err;
	}
	new_sl->next = env->explored_states[insn_idx];
	env->explored_states[insn_idx] = new_sl;
	 
	cur->parent = &new_sl->state;
	 
	for (i = 0; i < BPF_REG_FP; i++)
		cur->regs[i].live = REG_LIVE_NONE;
	for (i = 0; i < cur->allocated_stack / BPF_REG_SIZE; i++)
		if (cur->stack[i].slot_type[0] == STACK_SPILL)
			cur->stack[i].spilled_ptr.live = REG_LIVE_NONE;
	return 0;
}
