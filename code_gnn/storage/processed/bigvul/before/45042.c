int x86_emulate_insn(struct x86_emulate_ctxt *ctxt)
{
	const struct x86_emulate_ops *ops = ctxt->ops;
	int rc = X86EMUL_CONTINUE;
	int saved_dst_type = ctxt->dst.type;

	ctxt->mem_read.pos = 0;

	 
	if (ctxt->lock_prefix && (!(ctxt->d & Lock) || ctxt->dst.type != OP_MEM)) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	if ((ctxt->d & SrcMask) == SrcMemFAddr && ctxt->src.type != OP_MEM) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	if (unlikely(ctxt->d &
		     (No64|Undefined|Sse|Mmx|Intercept|CheckPerm|Priv|Prot|String))) {
		if ((ctxt->mode == X86EMUL_MODE_PROT64 && (ctxt->d & No64)) ||
				(ctxt->d & Undefined)) {
			rc = emulate_ud(ctxt);
			goto done;
		}

		if (((ctxt->d & (Sse|Mmx)) && ((ops->get_cr(ctxt, 0) & X86_CR0_EM)))
		    || ((ctxt->d & Sse) && !(ops->get_cr(ctxt, 4) & X86_CR4_OSFXSR))) {
			rc = emulate_ud(ctxt);
			goto done;
		}

		if ((ctxt->d & (Sse|Mmx)) && (ops->get_cr(ctxt, 0) & X86_CR0_TS)) {
			rc = emulate_nm(ctxt);
			goto done;
		}

		if (ctxt->d & Mmx) {
			rc = flush_pending_x87_faults(ctxt);
			if (rc != X86EMUL_CONTINUE)
				goto done;
			 
			fetch_possible_mmx_operand(ctxt, &ctxt->src);
			fetch_possible_mmx_operand(ctxt, &ctxt->src2);
			if (!(ctxt->d & Mov))
				fetch_possible_mmx_operand(ctxt, &ctxt->dst);
		}

		if (unlikely(ctxt->guest_mode) && (ctxt->d & Intercept)) {
			rc = emulator_check_intercept(ctxt, ctxt->intercept,
						      X86_ICPT_PRE_EXCEPT);
			if (rc != X86EMUL_CONTINUE)
				goto done;
		}

		 
		if ((ctxt->d & Prot) && ctxt->mode < X86EMUL_MODE_PROT16) {
			rc = emulate_ud(ctxt);
			goto done;
		}

		 
		if ((ctxt->d & Priv) && ops->cpl(ctxt)) {
			if (ctxt->d & PrivUD)
				rc = emulate_ud(ctxt);
			else
				rc = emulate_gp(ctxt, 0);
			goto done;
		}

		 
		if (ctxt->d & CheckPerm) {
			rc = ctxt->check_perm(ctxt);
			if (rc != X86EMUL_CONTINUE)
				goto done;
		}

		if (unlikely(ctxt->guest_mode) && (ctxt->d & Intercept)) {
			rc = emulator_check_intercept(ctxt, ctxt->intercept,
						      X86_ICPT_POST_EXCEPT);
			if (rc != X86EMUL_CONTINUE)
				goto done;
		}

		if (ctxt->rep_prefix && (ctxt->d & String)) {
			 
			if (address_mask(ctxt, reg_read(ctxt, VCPU_REGS_RCX)) == 0) {
				ctxt->eip = ctxt->_eip;
				ctxt->eflags &= ~EFLG_RF;
				goto done;
			}
		}
	}

	if ((ctxt->src.type == OP_MEM) && !(ctxt->d & NoAccess)) {
		rc = segmented_read(ctxt, ctxt->src.addr.mem,
				    ctxt->src.valptr, ctxt->src.bytes);
		if (rc != X86EMUL_CONTINUE)
			goto done;
		ctxt->src.orig_val64 = ctxt->src.val64;
	}

	if (ctxt->src2.type == OP_MEM) {
		rc = segmented_read(ctxt, ctxt->src2.addr.mem,
				    &ctxt->src2.val, ctxt->src2.bytes);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if ((ctxt->d & DstMask) == ImplicitOps)
		goto special_insn;


	if ((ctxt->dst.type == OP_MEM) && !(ctxt->d & Mov)) {
		 
		rc = segmented_read(ctxt, ctxt->dst.addr.mem,
				   &ctxt->dst.val, ctxt->dst.bytes);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}
	ctxt->dst.orig_val = ctxt->dst.val;

special_insn:

	if (unlikely(ctxt->guest_mode) && (ctxt->d & Intercept)) {
		rc = emulator_check_intercept(ctxt, ctxt->intercept,
					      X86_ICPT_POST_MEMACCESS);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if (ctxt->rep_prefix && (ctxt->d & String))
		ctxt->eflags |= EFLG_RF;
	else
		ctxt->eflags &= ~EFLG_RF;

	if (ctxt->execute) {
		if (ctxt->d & Fastop) {
			void (*fop)(struct fastop *) = (void *)ctxt->execute;
			rc = fastop(ctxt, fop);
			if (rc != X86EMUL_CONTINUE)
				goto done;
			goto writeback;
		}
		rc = ctxt->execute(ctxt);
		if (rc != X86EMUL_CONTINUE)
			goto done;
		goto writeback;
	}

	if (ctxt->opcode_len == 2)
		goto twobyte_insn;
	else if (ctxt->opcode_len == 3)
		goto threebyte_insn;

	switch (ctxt->b) {
	case 0x63:		 
		if (ctxt->mode != X86EMUL_MODE_PROT64)
			goto cannot_emulate;
		ctxt->dst.val = (s32) ctxt->src.val;
		break;
	case 0x70 ... 0x7f:  
		if (test_cc(ctxt->b, ctxt->eflags))
			rc = jmp_rel(ctxt, ctxt->src.val);
		break;
	case 0x8d:  
		ctxt->dst.val = ctxt->src.addr.mem.ea;
		break;
	case 0x90 ... 0x97:  
		if (ctxt->dst.addr.reg == reg_rmw(ctxt, VCPU_REGS_RAX))
			ctxt->dst.type = OP_NONE;
		else
			rc = em_xchg(ctxt);
		break;
	case 0x98:  
		switch (ctxt->op_bytes) {
		case 2: ctxt->dst.val = (s8)ctxt->dst.val; break;
		case 4: ctxt->dst.val = (s16)ctxt->dst.val; break;
		case 8: ctxt->dst.val = (s32)ctxt->dst.val; break;
		}
		break;
	case 0xcc:		 
		rc = emulate_int(ctxt, 3);
		break;
	case 0xcd:		 
		rc = emulate_int(ctxt, ctxt->src.val);
		break;
	case 0xce:		 
		if (ctxt->eflags & EFLG_OF)
			rc = emulate_int(ctxt, 4);
		break;
	case 0xe9:  
	case 0xeb:  
		rc = jmp_rel(ctxt, ctxt->src.val);
		ctxt->dst.type = OP_NONE;  
		break;
	case 0xf4:               
		ctxt->ops->halt(ctxt);
		break;
	case 0xf5:	 
		 
		ctxt->eflags ^= EFLG_CF;
		break;
	case 0xf8:  
		ctxt->eflags &= ~EFLG_CF;
		break;
	case 0xf9:  
		ctxt->eflags |= EFLG_CF;
		break;
	case 0xfc:  
		ctxt->eflags &= ~EFLG_DF;
		break;
	case 0xfd:  
		ctxt->eflags |= EFLG_DF;
		break;
	default:
		goto cannot_emulate;
	}

	if (rc != X86EMUL_CONTINUE)
		goto done;

writeback:
	if (ctxt->d & SrcWrite) {
		BUG_ON(ctxt->src.type == OP_MEM || ctxt->src.type == OP_MEM_STR);
		rc = writeback(ctxt, &ctxt->src);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}
	if (!(ctxt->d & NoWrite)) {
		rc = writeback(ctxt, &ctxt->dst);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	 
	ctxt->dst.type = saved_dst_type;

	if ((ctxt->d & SrcMask) == SrcSI)
		string_addr_inc(ctxt, VCPU_REGS_RSI, &ctxt->src);

	if ((ctxt->d & DstMask) == DstDI)
		string_addr_inc(ctxt, VCPU_REGS_RDI, &ctxt->dst);

	if (ctxt->rep_prefix && (ctxt->d & String)) {
		unsigned int count;
		struct read_cache *r = &ctxt->io_read;
		if ((ctxt->d & SrcMask) == SrcSI)
			count = ctxt->src.count;
		else
			count = ctxt->dst.count;
		register_address_increment(ctxt, VCPU_REGS_RCX, -count);

		if (!string_insn_completed(ctxt)) {
			 
			if ((r->end != 0 || reg_read(ctxt, VCPU_REGS_RCX) & 0x3ff) &&
			    (r->end == 0 || r->end != r->pos)) {
				 
				ctxt->mem_read.end = 0;
				writeback_registers(ctxt);
				return EMULATION_RESTART;
			}
			goto done;  
		}
		ctxt->eflags &= ~EFLG_RF;
	}

	ctxt->eip = ctxt->_eip;

done:
	if (rc == X86EMUL_PROPAGATE_FAULT) {
		WARN_ON(ctxt->exception.vector > 0x1f);
		ctxt->have_exception = true;
	}
	if (rc == X86EMUL_INTERCEPTED)
		return EMULATION_INTERCEPTED;

	if (rc == X86EMUL_CONTINUE)
		writeback_registers(ctxt);

	return (rc == X86EMUL_UNHANDLEABLE) ? EMULATION_FAILED : EMULATION_OK;

twobyte_insn:
	switch (ctxt->b) {
	case 0x09:		 
		(ctxt->ops->wbinvd)(ctxt);
		break;
	case 0x08:		 
	case 0x0d:		 
	case 0x18:		 
	case 0x1f:		 
		break;
	case 0x20:  
		ctxt->dst.val = ops->get_cr(ctxt, ctxt->modrm_reg);
		break;
	case 0x21:  
		ops->get_dr(ctxt, ctxt->modrm_reg, &ctxt->dst.val);
		break;
	case 0x40 ... 0x4f:	 
		if (test_cc(ctxt->b, ctxt->eflags))
			ctxt->dst.val = ctxt->src.val;
		else if (ctxt->mode != X86EMUL_MODE_PROT64 ||
			 ctxt->op_bytes != 4)
			ctxt->dst.type = OP_NONE;  
		break;
	case 0x80 ... 0x8f:  
		if (test_cc(ctxt->b, ctxt->eflags))
			rc = jmp_rel(ctxt, ctxt->src.val);
		break;
	case 0x90 ... 0x9f:      
		ctxt->dst.val = test_cc(ctxt->b, ctxt->eflags);
		break;
	case 0xb6 ... 0xb7:	 
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->src.bytes == 1) ? (u8) ctxt->src.val
						       : (u16) ctxt->src.val;
		break;
	case 0xbe ... 0xbf:	 
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->src.bytes == 1) ? (s8) ctxt->src.val :
							(s16) ctxt->src.val;
		break;
	default:
		goto cannot_emulate;
	}

threebyte_insn:

	if (rc != X86EMUL_CONTINUE)
		goto done;

	goto writeback;

cannot_emulate:
	return EMULATION_FAILED;
}
