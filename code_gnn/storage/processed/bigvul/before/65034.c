static int check_func_arg(struct bpf_verifier_env *env, u32 regno,
			  enum bpf_arg_type arg_type,
			  struct bpf_call_arg_meta *meta)
{
	struct bpf_reg_state *regs = env->cur_state.regs, *reg = &regs[regno];
	enum bpf_reg_type expected_type, type = reg->type;
	int err = 0;

	if (arg_type == ARG_DONTCARE)
		return 0;

	if (type == NOT_INIT) {
		verbose("R%d !read_ok\n", regno);
		return -EACCES;
	}

	if (arg_type == ARG_ANYTHING) {
		if (is_pointer_value(env, regno)) {
			verbose("R%d leaks addr into helper function\n", regno);
			return -EACCES;
		}
		return 0;
	}

	if (type == PTR_TO_PACKET &&
	    !may_access_direct_pkt_data(env, meta, BPF_READ)) {
		verbose("helper access to the packet is not allowed\n");
		return -EACCES;
	}

	if (arg_type == ARG_PTR_TO_MAP_KEY ||
	    arg_type == ARG_PTR_TO_MAP_VALUE) {
		expected_type = PTR_TO_STACK;
		if (type != PTR_TO_PACKET && type != expected_type)
			goto err_type;
	} else if (arg_type == ARG_CONST_SIZE ||
		   arg_type == ARG_CONST_SIZE_OR_ZERO) {
		expected_type = CONST_IMM;
		 
		if (type != UNKNOWN_VALUE && type != expected_type)
			goto err_type;
	} else if (arg_type == ARG_CONST_MAP_PTR) {
		expected_type = CONST_PTR_TO_MAP;
		if (type != expected_type)
			goto err_type;
	} else if (arg_type == ARG_PTR_TO_CTX) {
		expected_type = PTR_TO_CTX;
		if (type != expected_type)
			goto err_type;
	} else if (arg_type == ARG_PTR_TO_MEM ||
		   arg_type == ARG_PTR_TO_UNINIT_MEM) {
		expected_type = PTR_TO_STACK;
		 
		if (type == CONST_IMM && reg->imm == 0)
			 ;
		else if (type != PTR_TO_PACKET && type != PTR_TO_MAP_VALUE &&
			 type != PTR_TO_MAP_VALUE_ADJ && type != expected_type)
			goto err_type;
		meta->raw_mode = arg_type == ARG_PTR_TO_UNINIT_MEM;
	} else {
		verbose("unsupported arg_type %d\n", arg_type);
		return -EFAULT;
	}

	if (arg_type == ARG_CONST_MAP_PTR) {
		 
		meta->map_ptr = reg->map_ptr;
	} else if (arg_type == ARG_PTR_TO_MAP_KEY) {
		 
		if (!meta->map_ptr) {
			 
			verbose("invalid map_ptr to access map->key\n");
			return -EACCES;
		}
		if (type == PTR_TO_PACKET)
			err = check_packet_access(env, regno, 0,
						  meta->map_ptr->key_size);
		else
			err = check_stack_boundary(env, regno,
						   meta->map_ptr->key_size,
						   false, NULL);
	} else if (arg_type == ARG_PTR_TO_MAP_VALUE) {
		 
		if (!meta->map_ptr) {
			 
			verbose("invalid map_ptr to access map->value\n");
			return -EACCES;
		}
		if (type == PTR_TO_PACKET)
			err = check_packet_access(env, regno, 0,
						  meta->map_ptr->value_size);
		else
			err = check_stack_boundary(env, regno,
						   meta->map_ptr->value_size,
						   false, NULL);
	} else if (arg_type == ARG_CONST_SIZE ||
		   arg_type == ARG_CONST_SIZE_OR_ZERO) {
		bool zero_size_allowed = (arg_type == ARG_CONST_SIZE_OR_ZERO);

		 
		if (regno == 0) {
			 
			verbose("ARG_CONST_SIZE cannot be first argument\n");
			return -EACCES;
		}

		 
		if (type == UNKNOWN_VALUE) {
			 
			meta = NULL;

			if (reg->min_value < 0) {
				verbose("R%d min value is negative, either use unsigned or 'var &= const'\n",
					regno);
				return -EACCES;
			}

			if (reg->min_value == 0) {
				err = check_helper_mem_access(env, regno - 1, 0,
							      zero_size_allowed,
							      meta);
				if (err)
					return err;
			}

			if (reg->max_value == BPF_REGISTER_MAX_RANGE) {
				verbose("R%d unbounded memory access, use 'var &= const' or 'if (var < const)'\n",
					regno);
				return -EACCES;
			}
			err = check_helper_mem_access(env, regno - 1,
						      reg->max_value,
						      zero_size_allowed, meta);
			if (err)
				return err;
		} else {
			 
			err = check_helper_mem_access(env, regno - 1, reg->imm,
						      zero_size_allowed, meta);
		}
	}

	return err;
err_type:
	verbose("R%d type=%s expected=%s\n", regno,
		reg_type_str[type], reg_type_str[expected_type]);
	return -EACCES;
}