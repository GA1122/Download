static int adjust_reg_min_max_vals(struct bpf_verifier_env *env,
				   struct bpf_insn *insn)
{
	struct bpf_reg_state *regs = cur_regs(env), *dst_reg, *src_reg;
	struct bpf_reg_state *ptr_reg = NULL, off_reg = {0};
	u8 opcode = BPF_OP(insn->code);
	int rc;

	dst_reg = &regs[insn->dst_reg];
	src_reg = NULL;
	if (dst_reg->type != SCALAR_VALUE)
		ptr_reg = dst_reg;
	if (BPF_SRC(insn->code) == BPF_X) {
		src_reg = &regs[insn->src_reg];
		if (src_reg->type != SCALAR_VALUE) {
			if (dst_reg->type != SCALAR_VALUE) {
				 
				if (!env->allow_ptr_leaks) {
					verbose(env, "R%d pointer %s pointer prohibited\n",
						insn->dst_reg,
						bpf_alu_string[opcode >> 4]);
					return -EACCES;
				}
				mark_reg_unknown(env, regs, insn->dst_reg);
				return 0;
			} else {
				 
				rc = adjust_ptr_min_max_vals(env, insn,
							     src_reg, dst_reg);
				if (rc == -EACCES && env->allow_ptr_leaks) {
					 
					__mark_reg_unknown(&off_reg);
					return adjust_scalar_min_max_vals(
							env, insn,
							dst_reg, off_reg);
				}
				return rc;
			}
		} else if (ptr_reg) {
			 
			rc = adjust_ptr_min_max_vals(env, insn,
						     dst_reg, src_reg);
			if (rc == -EACCES && env->allow_ptr_leaks) {
				 
				__mark_reg_unknown(dst_reg);
				return adjust_scalar_min_max_vals(
						env, insn, dst_reg, *src_reg);
			}
			return rc;
		}
	} else {
		 
		off_reg.type = SCALAR_VALUE;
		__mark_reg_known(&off_reg, insn->imm);
		src_reg = &off_reg;
		if (ptr_reg) {  
			rc = adjust_ptr_min_max_vals(env, insn,
						     ptr_reg, src_reg);
			if (rc == -EACCES && env->allow_ptr_leaks) {
				 
				__mark_reg_unknown(dst_reg);
				return adjust_scalar_min_max_vals(
						env, insn, dst_reg, off_reg);
			}
			return rc;
		}
	}

	 
	if (WARN_ON_ONCE(ptr_reg)) {
		print_verifier_state(env, env->cur_state);
		verbose(env, "verifier internal error: unexpected ptr_reg\n");
		return -EINVAL;
	}
	if (WARN_ON(!src_reg)) {
		print_verifier_state(env, env->cur_state);
		verbose(env, "verifier internal error: no src_reg\n");
		return -EINVAL;
	}
	return adjust_scalar_min_max_vals(env, insn, dst_reg, *src_reg);
}
