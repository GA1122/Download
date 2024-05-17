 static int adjust_ptr_min_max_vals(struct bpf_verifier_env *env,
 				   struct bpf_insn *insn,
 				   const struct bpf_reg_state *ptr_reg,
 				   const struct bpf_reg_state *off_reg)
 {
 	struct bpf_verifier_state *vstate = env->cur_state;
 	struct bpf_func_state *state = vstate->frame[vstate->curframe];
 	struct bpf_reg_state *regs = state->regs, *dst_reg;
 	bool known = tnum_is_const(off_reg->var_off);
 	s64 smin_val = off_reg->smin_value, smax_val = off_reg->smax_value,
 	    smin_ptr = ptr_reg->smin_value, smax_ptr = ptr_reg->smax_value;
 	u64 umin_val = off_reg->umin_value, umax_val = off_reg->umax_value,
  	    umin_ptr = ptr_reg->umin_value, umax_ptr = ptr_reg->umax_value;
  	u32 dst = insn->dst_reg, src = insn->src_reg;
  	u8 opcode = BPF_OP(insn->code);
// 	int ret;
  
  	dst_reg = &regs[dst];
  
 	if ((known && (smin_val != smax_val || umin_val != umax_val)) ||
 	    smin_val > smax_val || umin_val > umax_val) {
 		 
 		__mark_reg_unknown(dst_reg);
 		return 0;
 	}
 
 	if (BPF_CLASS(insn->code) != BPF_ALU64) {
 		 
 		verbose(env,
 			"R%d 32-bit pointer arithmetic prohibited\n",
 			dst);
 		return -EACCES;
 	}
 
 	switch (ptr_reg->type) {
 	case PTR_TO_MAP_VALUE_OR_NULL:
 		verbose(env, "R%d pointer arithmetic on %s prohibited, null-check it first\n",
 			dst, reg_type_str[ptr_reg->type]);
 		return -EACCES;
 	case CONST_PTR_TO_MAP:
 	case PTR_TO_PACKET_END:
 	case PTR_TO_SOCKET:
 	case PTR_TO_SOCKET_OR_NULL:
 		verbose(env, "R%d pointer arithmetic on %s prohibited\n",
 			dst, reg_type_str[ptr_reg->type]);
 		return -EACCES;
 	case PTR_TO_MAP_VALUE:
 		if (!env->allow_ptr_leaks && !known && (smin_val < 0) != (smax_val < 0)) {
 			verbose(env, "R%d has unknown scalar with mixed signed bounds, pointer arithmetic with it prohibited for !root\n",
 				off_reg == dst_reg ? dst : src);
 			return -EACCES;
 		}
 		 
 	default:
 		break;
 	}
 
 	 
 	dst_reg->type = ptr_reg->type;
 	dst_reg->id = ptr_reg->id;
 
 	if (!check_reg_sane_offset(env, off_reg, ptr_reg->type) ||
 	    !check_reg_sane_offset(env, ptr_reg, ptr_reg->type))
 		return -EINVAL;
  
  	switch (opcode) {
  	case BPF_ADD:
// 		ret = sanitize_ptr_alu(env, insn, ptr_reg, dst_reg, smin_val < 0);
// 		if (ret < 0) {
// 			verbose(env, "R%d tried to add from different maps or paths\n", dst);
// 			return ret;
// 		}
  		 
 		if (known && (ptr_reg->off + smin_val ==
 			      (s64)(s32)(ptr_reg->off + smin_val))) {
 			 
 			dst_reg->smin_value = smin_ptr;
 			dst_reg->smax_value = smax_ptr;
 			dst_reg->umin_value = umin_ptr;
 			dst_reg->umax_value = umax_ptr;
 			dst_reg->var_off = ptr_reg->var_off;
 			dst_reg->off = ptr_reg->off + smin_val;
 			dst_reg->raw = ptr_reg->raw;
 			break;
 		}
 		 
 		if (signed_add_overflows(smin_ptr, smin_val) ||
 		    signed_add_overflows(smax_ptr, smax_val)) {
 			dst_reg->smin_value = S64_MIN;
 			dst_reg->smax_value = S64_MAX;
 		} else {
 			dst_reg->smin_value = smin_ptr + smin_val;
 			dst_reg->smax_value = smax_ptr + smax_val;
 		}
 		if (umin_ptr + umin_val < umin_ptr ||
 		    umax_ptr + umax_val < umax_ptr) {
 			dst_reg->umin_value = 0;
 			dst_reg->umax_value = U64_MAX;
 		} else {
 			dst_reg->umin_value = umin_ptr + umin_val;
 			dst_reg->umax_value = umax_ptr + umax_val;
 		}
 		dst_reg->var_off = tnum_add(ptr_reg->var_off, off_reg->var_off);
 		dst_reg->off = ptr_reg->off;
 		dst_reg->raw = ptr_reg->raw;
 		if (reg_is_pkt_pointer(ptr_reg)) {
 			dst_reg->id = ++env->id_gen;
 			 
 			dst_reg->raw = 0;
  		}
  		break;
  	case BPF_SUB:
// 		ret = sanitize_ptr_alu(env, insn, ptr_reg, dst_reg, smin_val < 0);
// 		if (ret < 0) {
// 			verbose(env, "R%d tried to sub from different maps or paths\n", dst);
// 			return ret;
// 		}
  		if (dst_reg == off_reg) {
  			 
  			verbose(env, "R%d tried to subtract pointer from scalar\n",
 				dst);
 			return -EACCES;
 		}
 		 
 		if (ptr_reg->type == PTR_TO_STACK) {
 			verbose(env, "R%d subtraction from stack pointer prohibited\n",
 				dst);
 			return -EACCES;
 		}
 		if (known && (ptr_reg->off - smin_val ==
 			      (s64)(s32)(ptr_reg->off - smin_val))) {
 			 
 			dst_reg->smin_value = smin_ptr;
 			dst_reg->smax_value = smax_ptr;
 			dst_reg->umin_value = umin_ptr;
 			dst_reg->umax_value = umax_ptr;
 			dst_reg->var_off = ptr_reg->var_off;
 			dst_reg->id = ptr_reg->id;
 			dst_reg->off = ptr_reg->off - smin_val;
 			dst_reg->raw = ptr_reg->raw;
 			break;
 		}
 		 
 		if (signed_sub_overflows(smin_ptr, smax_val) ||
 		    signed_sub_overflows(smax_ptr, smin_val)) {
 			 
 			dst_reg->smin_value = S64_MIN;
 			dst_reg->smax_value = S64_MAX;
 		} else {
 			dst_reg->smin_value = smin_ptr - smax_val;
 			dst_reg->smax_value = smax_ptr - smin_val;
 		}
 		if (umin_ptr < umax_val) {
 			 
 			dst_reg->umin_value = 0;
 			dst_reg->umax_value = U64_MAX;
 		} else {
 			 
 			dst_reg->umin_value = umin_ptr - umax_val;
 			dst_reg->umax_value = umax_ptr - umin_val;
 		}
 		dst_reg->var_off = tnum_sub(ptr_reg->var_off, off_reg->var_off);
 		dst_reg->off = ptr_reg->off;
 		dst_reg->raw = ptr_reg->raw;
 		if (reg_is_pkt_pointer(ptr_reg)) {
 			dst_reg->id = ++env->id_gen;
 			 
 			if (smin_val < 0)
 				dst_reg->raw = 0;
 		}
 		break;
 	case BPF_AND:
 	case BPF_OR:
 	case BPF_XOR:
 		 
 		verbose(env, "R%d bitwise operator %s on pointer prohibited\n",
 			dst, bpf_alu_string[opcode >> 4]);
 		return -EACCES;
 	default:
 		 
 		verbose(env, "R%d pointer arithmetic with %s operator prohibited\n",
 			dst, bpf_alu_string[opcode >> 4]);
 		return -EACCES;
 	}
 
 	if (!check_reg_sane_offset(env, dst_reg, ptr_reg->type))
 		return -EINVAL;
 
 	__update_reg_bounds(dst_reg);
 	__reg_deduce_bounds(dst_reg);
 	__reg_bound_offset(dst_reg);
 
 	 
 	if (!env->allow_ptr_leaks) {
 		if (dst_reg->type == PTR_TO_MAP_VALUE &&
 		    check_map_access(env, dst, dst_reg->off, 1, false)) {
 			verbose(env, "R%d pointer arithmetic of map value goes out of range, "
 				"prohibited for !root\n", dst);
 			return -EACCES;
 		} else if (dst_reg->type == PTR_TO_STACK &&
 			   check_stack_access(env, dst_reg, dst_reg->off +
 					      dst_reg->var_off.value, 1)) {
 			verbose(env, "R%d stack pointer arithmetic goes out of range, "
 				"prohibited for !root\n", dst);
 			return -EACCES;
 		}
 	}
 
 	return 0;
 }