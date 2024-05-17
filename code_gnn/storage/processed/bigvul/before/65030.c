static int check_call(struct bpf_verifier_env *env, int func_id, int insn_idx)
{
	struct bpf_verifier_state *state = &env->cur_state;
	const struct bpf_func_proto *fn = NULL;
	struct bpf_reg_state *regs = state->regs;
	struct bpf_reg_state *reg;
	struct bpf_call_arg_meta meta;
	bool changes_data;
	int i, err;

	 
	if (func_id < 0 || func_id >= __BPF_FUNC_MAX_ID) {
		verbose("invalid func %s#%d\n", func_id_name(func_id), func_id);
		return -EINVAL;
	}

	if (env->prog->aux->ops->get_func_proto)
		fn = env->prog->aux->ops->get_func_proto(func_id);

	if (!fn) {
		verbose("unknown func %s#%d\n", func_id_name(func_id), func_id);
		return -EINVAL;
	}

	 
	if (!env->prog->gpl_compatible && fn->gpl_only) {
		verbose("cannot call GPL only function from proprietary program\n");
		return -EINVAL;
	}

	changes_data = bpf_helper_changes_pkt_data(fn->func);

	memset(&meta, 0, sizeof(meta));
	meta.pkt_access = fn->pkt_access;

	 
	err = check_raw_mode(fn);
	if (err) {
		verbose("kernel subsystem misconfigured func %s#%d\n",
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

	 
	for (i = 0; i < meta.access_size; i++) {
		err = check_mem_access(env, meta.regno, i, BPF_B, BPF_WRITE, -1);
		if (err)
			return err;
	}

	 
	for (i = 0; i < CALLER_SAVED_REGS; i++) {
		reg = regs + caller_saved[i];
		reg->type = NOT_INIT;
		reg->imm = 0;
	}

	 
	if (fn->ret_type == RET_INTEGER) {
		regs[BPF_REG_0].type = UNKNOWN_VALUE;
	} else if (fn->ret_type == RET_VOID) {
		regs[BPF_REG_0].type = NOT_INIT;
	} else if (fn->ret_type == RET_PTR_TO_MAP_VALUE_OR_NULL) {
		struct bpf_insn_aux_data *insn_aux;

		regs[BPF_REG_0].type = PTR_TO_MAP_VALUE_OR_NULL;
		regs[BPF_REG_0].max_value = regs[BPF_REG_0].min_value = 0;
		 
		if (meta.map_ptr == NULL) {
			verbose("kernel subsystem misconfigured verifier\n");
			return -EINVAL;
		}
		regs[BPF_REG_0].map_ptr = meta.map_ptr;
		regs[BPF_REG_0].id = ++env->id_gen;
		insn_aux = &env->insn_aux_data[insn_idx];
		if (!insn_aux->map_ptr)
			insn_aux->map_ptr = meta.map_ptr;
		else if (insn_aux->map_ptr != meta.map_ptr)
			insn_aux->map_ptr = BPF_MAP_PTR_POISON;
	} else {
		verbose("unknown return type %d of func %s#%d\n",
			fn->ret_type, func_id_name(func_id), func_id);
		return -EINVAL;
	}

	err = check_map_func_compatibility(meta.map_ptr, func_id);
	if (err)
		return err;

	if (changes_data)
		clear_all_pkt_pointers(env);
	return 0;
}