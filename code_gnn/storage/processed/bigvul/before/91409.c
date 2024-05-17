static int check_helper_call(struct bpf_verifier_env *env, int func_id, int insn_idx)
{
	const struct bpf_func_proto *fn = NULL;
	struct bpf_reg_state *regs;
	struct bpf_call_arg_meta meta;
	bool changes_data;
	int i, err;

	 
	if (func_id < 0 || func_id >= __BPF_FUNC_MAX_ID) {
		verbose(env, "invalid func %s#%d\n", func_id_name(func_id),
			func_id);
		return -EINVAL;
	}

	if (env->ops->get_func_proto)
		fn = env->ops->get_func_proto(func_id, env->prog);
	if (!fn) {
		verbose(env, "unknown func %s#%d\n", func_id_name(func_id),
			func_id);
		return -EINVAL;
	}

	 
	if (!env->prog->gpl_compatible && fn->gpl_only) {
		verbose(env, "cannot call GPL-restricted function from non-GPL compatible program\n");
		return -EINVAL;
	}

	 
	changes_data = bpf_helper_changes_pkt_data(fn->func);
	if (changes_data && fn->arg1_type != ARG_PTR_TO_CTX) {
		verbose(env, "kernel subsystem misconfigured func %s#%d: r1 != ctx\n",
			func_id_name(func_id), func_id);
		return -EINVAL;
	}

	memset(&meta, 0, sizeof(meta));
	meta.pkt_access = fn->pkt_access;

	err = check_func_proto(fn);
	if (err) {
		verbose(env, "kernel subsystem misconfigured func %s#%d\n",
			func_id_name(func_id), func_id);
		return err;
	}

	 
	err = check_func_arg(env, BPF_REG_1, fn->arg1_type, &meta);
	if (err)
		return err;
	err = check_func_arg(env, BPF_REG_2, fn->arg2_type, &meta);
	if (err)
		return err;
	err = check_func_arg(env, BPF_REG_3, fn->arg3_type, &meta);
	if (err)
		return err;
	err = check_func_arg(env, BPF_REG_4, fn->arg4_type, &meta);
	if (err)
		return err;
	err = check_func_arg(env, BPF_REG_5, fn->arg5_type, &meta);
	if (err)
		return err;

	err = record_func_map(env, &meta, func_id, insn_idx);
	if (err)
		return err;

	 
	for (i = 0; i < meta.access_size; i++) {
		err = check_mem_access(env, insn_idx, meta.regno, i, BPF_B,
				       BPF_WRITE, -1, false);
		if (err)
			return err;
	}

	if (func_id == BPF_FUNC_tail_call) {
		err = check_reference_leak(env);
		if (err) {
			verbose(env, "tail_call would lead to reference leak\n");
			return err;
		}
	} else if (is_release_function(func_id)) {
		err = release_reference(env, &meta);
		if (err)
			return err;
	}

	regs = cur_regs(env);

	 
	if (func_id == BPF_FUNC_get_local_storage &&
	    !register_is_null(&regs[BPF_REG_2])) {
		verbose(env, "get_local_storage() doesn't support non-zero flags\n");
		return -EINVAL;
	}

	 
	for (i = 0; i < CALLER_SAVED_REGS; i++) {
		mark_reg_not_init(env, regs, caller_saved[i]);
		check_reg_arg(env, caller_saved[i], DST_OP_NO_MARK);
	}

	 
	if (fn->ret_type == RET_INTEGER) {
		 
		mark_reg_unknown(env, regs, BPF_REG_0);
	} else if (fn->ret_type == RET_VOID) {
		regs[BPF_REG_0].type = NOT_INIT;
	} else if (fn->ret_type == RET_PTR_TO_MAP_VALUE_OR_NULL ||
		   fn->ret_type == RET_PTR_TO_MAP_VALUE) {
		 
		mark_reg_known_zero(env, regs, BPF_REG_0);
		 
		if (meta.map_ptr == NULL) {
			verbose(env,
				"kernel subsystem misconfigured verifier\n");
			return -EINVAL;
		}
		regs[BPF_REG_0].map_ptr = meta.map_ptr;
		if (fn->ret_type == RET_PTR_TO_MAP_VALUE) {
			regs[BPF_REG_0].type = PTR_TO_MAP_VALUE;
		} else {
			regs[BPF_REG_0].type = PTR_TO_MAP_VALUE_OR_NULL;
			regs[BPF_REG_0].id = ++env->id_gen;
		}
	} else if (fn->ret_type == RET_PTR_TO_SOCKET_OR_NULL) {
		int id = acquire_reference_state(env, insn_idx);
		if (id < 0)
			return id;
		mark_reg_known_zero(env, regs, BPF_REG_0);
		regs[BPF_REG_0].type = PTR_TO_SOCKET_OR_NULL;
		regs[BPF_REG_0].id = id;
	} else {
		verbose(env, "unknown return type %d of func %s#%d\n",
			fn->ret_type, func_id_name(func_id), func_id);
		return -EINVAL;
	}

	do_refine_retval_range(regs, fn->ret_type, func_id, &meta);

	err = check_map_func_compatibility(env, meta.map_ptr, func_id);
	if (err)
		return err;

	if (func_id == BPF_FUNC_get_stack && !env->prog->has_callchain_buf) {
		const char *err_str;

#ifdef CONFIG_PERF_EVENTS
		err = get_callchain_buffers(sysctl_perf_event_max_stack);
		err_str = "cannot get callchain buffer for func %s#%d\n";
#else
		err = -ENOTSUPP;
		err_str = "func %s#%d not supported without CONFIG_PERF_EVENTS\n";
#endif
		if (err) {
			verbose(env, err_str, func_id_name(func_id), func_id);
			return err;
		}

		env->prog->has_callchain_buf = true;
	}

	if (changes_data)
		clear_all_pkt_pointers(env);
	return 0;
}
