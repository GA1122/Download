static int is_state_visited(struct bpf_verifier_env *env, int insn_idx)
{
	struct bpf_verifier_state_list *new_sl;
	struct bpf_verifier_state_list *sl;
	struct bpf_verifier_state *cur = env->cur_state, *new;
	int i, j, err, states_cnt = 0;

	sl = env->explored_states[insn_idx];
	if (!sl)
		 
		return 0;

	clean_live_states(env, insn_idx, cur);

	while (sl != STATE_LIST_MARK) {
		if (states_equal(env, &sl->state, cur)) {
			 
			err = propagate_liveness(env, &sl->state, cur);
			if (err)
				return err;
			return 1;
		}
		sl = sl->next;
		states_cnt++;
	}

	if (!env->allow_ptr_leaks && states_cnt > BPF_COMPLEXITY_LIMIT_STATES)
		return 0;

	 
	new_sl = kzalloc(sizeof(struct bpf_verifier_state_list), GFP_KERNEL);
	if (!new_sl)
		return -ENOMEM;

	 
	new = &new_sl->state;
	err = copy_verifier_state(new, cur);
	if (err) {
		free_verifier_state(new, false);
		kfree(new_sl);
		return err;
	}
	new_sl->next = env->explored_states[insn_idx];
	env->explored_states[insn_idx] = new_sl;
	 
	for (j = 0; j <= cur->curframe; j++)
		for (i = j < cur->curframe ? BPF_REG_6 : 0; i < BPF_REG_FP; i++)
			cur->frame[j]->regs[i].parent = &new->frame[j]->regs[i];
	 
	for (i = 0; i < BPF_REG_FP; i++)
		cur->frame[cur->curframe]->regs[i].live = REG_LIVE_NONE;

	 
	for (j = 0; j <= cur->curframe; j++) {
		struct bpf_func_state *frame = cur->frame[j];
		struct bpf_func_state *newframe = new->frame[j];

		for (i = 0; i < frame->allocated_stack / BPF_REG_SIZE; i++) {
			frame->stack[i].spilled_ptr.live = REG_LIVE_NONE;
			frame->stack[i].spilled_ptr.parent =
						&newframe->stack[i].spilled_ptr;
		}
	}
	return 0;
}
