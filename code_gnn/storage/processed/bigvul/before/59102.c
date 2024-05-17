static int adjust_scalar_min_max_vals(struct bpf_verifier_env *env,
				      struct bpf_insn *insn,
				      struct bpf_reg_state *dst_reg,
				      struct bpf_reg_state src_reg)
{
	struct bpf_reg_state *regs = cur_regs(env);
	u8 opcode = BPF_OP(insn->code);
	bool src_known, dst_known;
	s64 smin_val, smax_val;
	u64 umin_val, umax_val;

	if (BPF_CLASS(insn->code) != BPF_ALU64) {
		 
		coerce_reg_to_32(dst_reg);
		coerce_reg_to_32(&src_reg);
	}
	smin_val = src_reg.smin_value;
	smax_val = src_reg.smax_value;
	umin_val = src_reg.umin_value;
	umax_val = src_reg.umax_value;
	src_known = tnum_is_const(src_reg.var_off);
	dst_known = tnum_is_const(dst_reg->var_off);

	switch (opcode) {
	case BPF_ADD:
		if (signed_add_overflows(dst_reg->smin_value, smin_val) ||
		    signed_add_overflows(dst_reg->smax_value, smax_val)) {
			dst_reg->smin_value = S64_MIN;
			dst_reg->smax_value = S64_MAX;
		} else {
			dst_reg->smin_value += smin_val;
			dst_reg->smax_value += smax_val;
		}
		if (dst_reg->umin_value + umin_val < umin_val ||
		    dst_reg->umax_value + umax_val < umax_val) {
			dst_reg->umin_value = 0;
			dst_reg->umax_value = U64_MAX;
		} else {
			dst_reg->umin_value += umin_val;
			dst_reg->umax_value += umax_val;
		}
		dst_reg->var_off = tnum_add(dst_reg->var_off, src_reg.var_off);
		break;
	case BPF_SUB:
		if (signed_sub_overflows(dst_reg->smin_value, smax_val) ||
		    signed_sub_overflows(dst_reg->smax_value, smin_val)) {
			 
			dst_reg->smin_value = S64_MIN;
			dst_reg->smax_value = S64_MAX;
		} else {
			dst_reg->smin_value -= smax_val;
			dst_reg->smax_value -= smin_val;
		}
		if (dst_reg->umin_value < umax_val) {
			 
			dst_reg->umin_value = 0;
			dst_reg->umax_value = U64_MAX;
		} else {
			 
			dst_reg->umin_value -= umax_val;
			dst_reg->umax_value -= umin_val;
		}
		dst_reg->var_off = tnum_sub(dst_reg->var_off, src_reg.var_off);
		break;
	case BPF_MUL:
		dst_reg->var_off = tnum_mul(dst_reg->var_off, src_reg.var_off);
		if (smin_val < 0 || dst_reg->smin_value < 0) {
			 
			__mark_reg_unbounded(dst_reg);
			__update_reg_bounds(dst_reg);
			break;
		}
		 
		if (umax_val > U32_MAX || dst_reg->umax_value > U32_MAX) {
			 
			__mark_reg_unbounded(dst_reg);
			 
			__update_reg_bounds(dst_reg);
			break;
		}
		dst_reg->umin_value *= umin_val;
		dst_reg->umax_value *= umax_val;
		if (dst_reg->umax_value > S64_MAX) {
			 
			dst_reg->smin_value = S64_MIN;
			dst_reg->smax_value = S64_MAX;
		} else {
			dst_reg->smin_value = dst_reg->umin_value;
			dst_reg->smax_value = dst_reg->umax_value;
		}
		break;
	case BPF_AND:
		if (src_known && dst_known) {
			__mark_reg_known(dst_reg, dst_reg->var_off.value &
						  src_reg.var_off.value);
			break;
		}
		 
		dst_reg->var_off = tnum_and(dst_reg->var_off, src_reg.var_off);
		dst_reg->umin_value = dst_reg->var_off.value;
		dst_reg->umax_value = min(dst_reg->umax_value, umax_val);
		if (dst_reg->smin_value < 0 || smin_val < 0) {
			 
			dst_reg->smin_value = S64_MIN;
			dst_reg->smax_value = S64_MAX;
		} else {
			 
			dst_reg->smin_value = dst_reg->umin_value;
			dst_reg->smax_value = dst_reg->umax_value;
		}
		 
		__update_reg_bounds(dst_reg);
		break;
	case BPF_OR:
		if (src_known && dst_known) {
			__mark_reg_known(dst_reg, dst_reg->var_off.value |
						  src_reg.var_off.value);
			break;
		}
		 
		dst_reg->var_off = tnum_or(dst_reg->var_off, src_reg.var_off);
		dst_reg->umin_value = max(dst_reg->umin_value, umin_val);
		dst_reg->umax_value = dst_reg->var_off.value |
				      dst_reg->var_off.mask;
		if (dst_reg->smin_value < 0 || smin_val < 0) {
			 
			dst_reg->smin_value = S64_MIN;
			dst_reg->smax_value = S64_MAX;
		} else {
			 
			dst_reg->smin_value = dst_reg->umin_value;
			dst_reg->smax_value = dst_reg->umax_value;
		}
		 
		__update_reg_bounds(dst_reg);
		break;
	case BPF_LSH:
		if (umax_val > 63) {
			 
			mark_reg_unknown(env, regs, insn->dst_reg);
			break;
		}
		 
		dst_reg->smin_value = S64_MIN;
		dst_reg->smax_value = S64_MAX;
		 
		if (dst_reg->umax_value > 1ULL << (63 - umax_val)) {
			dst_reg->umin_value = 0;
			dst_reg->umax_value = U64_MAX;
		} else {
			dst_reg->umin_value <<= umin_val;
			dst_reg->umax_value <<= umax_val;
		}
		if (src_known)
			dst_reg->var_off = tnum_lshift(dst_reg->var_off, umin_val);
		else
			dst_reg->var_off = tnum_lshift(tnum_unknown, umin_val);
		 
		__update_reg_bounds(dst_reg);
		break;
	case BPF_RSH:
		if (umax_val > 63) {
			 
			mark_reg_unknown(env, regs, insn->dst_reg);
			break;
		}
		 
		if (dst_reg->smin_value < 0) {
			if (umin_val) {
				 
				dst_reg->smin_value = 0;
			} else {
				 
				dst_reg->smin_value = S64_MIN;
				dst_reg->smax_value = S64_MAX;
			}
		} else {
			dst_reg->smin_value =
				(u64)(dst_reg->smin_value) >> umax_val;
		}
		if (src_known)
			dst_reg->var_off = tnum_rshift(dst_reg->var_off,
						       umin_val);
		else
			dst_reg->var_off = tnum_rshift(tnum_unknown, umin_val);
		dst_reg->umin_value >>= umax_val;
		dst_reg->umax_value >>= umin_val;
		 
		__update_reg_bounds(dst_reg);
		break;
	default:
		mark_reg_unknown(env, regs, insn->dst_reg);
		break;
	}

	__reg_deduce_bounds(dst_reg);
	__reg_bound_offset(dst_reg);
	return 0;
}