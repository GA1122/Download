static int convert_ctx_accesses(struct bpf_verifier_env *env)
{
	const struct bpf_verifier_ops *ops = env->ops;
	int i, cnt, size, ctx_field_size, delta = 0;
	const int insn_cnt = env->prog->len;
	struct bpf_insn insn_buf[16], *insn;
	struct bpf_prog *new_prog;
	enum bpf_access_type type;
	bool is_narrower_load;
	u32 target_size;

	if (ops->gen_prologue) {
		cnt = ops->gen_prologue(insn_buf, env->seen_direct_write,
					env->prog);
		if (cnt >= ARRAY_SIZE(insn_buf)) {
			verbose(env, "bpf verifier is misconfigured\n");
			return -EINVAL;
		} else if (cnt) {
			new_prog = bpf_patch_insn_data(env, 0, insn_buf, cnt);
			if (!new_prog)
				return -ENOMEM;

			env->prog = new_prog;
			delta += cnt - 1;
		}
	}

	if (!ops->convert_ctx_access)
		return 0;

	insn = env->prog->insnsi + delta;

	for (i = 0; i < insn_cnt; i++, insn++) {
		if (insn->code == (BPF_LDX | BPF_MEM | BPF_B) ||
		    insn->code == (BPF_LDX | BPF_MEM | BPF_H) ||
		    insn->code == (BPF_LDX | BPF_MEM | BPF_W) ||
		    insn->code == (BPF_LDX | BPF_MEM | BPF_DW))
			type = BPF_READ;
		else if (insn->code == (BPF_STX | BPF_MEM | BPF_B) ||
			 insn->code == (BPF_STX | BPF_MEM | BPF_H) ||
			 insn->code == (BPF_STX | BPF_MEM | BPF_W) ||
			 insn->code == (BPF_STX | BPF_MEM | BPF_DW))
			type = BPF_WRITE;
		else
			continue;

		if (env->insn_aux_data[i + delta].ptr_type != PTR_TO_CTX)
			continue;

		ctx_field_size = env->insn_aux_data[i + delta].ctx_field_size;
		size = BPF_LDST_BYTES(insn);

		 
		is_narrower_load = size < ctx_field_size;
		if (is_narrower_load) {
			u32 off = insn->off;
			u8 size_code;

			if (type == BPF_WRITE) {
				verbose(env, "bpf verifier narrow ctx access misconfigured\n");
				return -EINVAL;
			}

			size_code = BPF_H;
			if (ctx_field_size == 4)
				size_code = BPF_W;
			else if (ctx_field_size == 8)
				size_code = BPF_DW;

			insn->off = off & ~(ctx_field_size - 1);
			insn->code = BPF_LDX | BPF_MEM | size_code;
		}

		target_size = 0;
		cnt = ops->convert_ctx_access(type, insn, insn_buf, env->prog,
					      &target_size);
		if (cnt == 0 || cnt >= ARRAY_SIZE(insn_buf) ||
		    (ctx_field_size && !target_size)) {
			verbose(env, "bpf verifier is misconfigured\n");
			return -EINVAL;
		}

		if (is_narrower_load && size < target_size) {
			if (ctx_field_size <= 4)
				insn_buf[cnt++] = BPF_ALU32_IMM(BPF_AND, insn->dst_reg,
								(1 << size * 8) - 1);
			else
				insn_buf[cnt++] = BPF_ALU64_IMM(BPF_AND, insn->dst_reg,
								(1 << size * 8) - 1);
		}

		new_prog = bpf_patch_insn_data(env, i + delta, insn_buf, cnt);
		if (!new_prog)
			return -ENOMEM;

		delta += cnt - 1;

		 
		env->prog = new_prog;
		insn      = new_prog->insnsi + i + delta;
	}

	return 0;
}