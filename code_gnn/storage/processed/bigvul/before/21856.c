int x86_emulate_insn(struct x86_emulate_ctxt *ctxt)
{
	struct x86_emulate_ops *ops = ctxt->ops;
	int rc = X86EMUL_CONTINUE;
	int saved_dst_type = ctxt->dst.type;

	ctxt->mem_read.pos = 0;

	if (ctxt->mode == X86EMUL_MODE_PROT64 && (ctxt->d & No64)) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	 
	if (ctxt->lock_prefix && (!(ctxt->d & Lock) || ctxt->dst.type != OP_MEM)) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	if ((ctxt->d & SrcMask) == SrcMemFAddr && ctxt->src.type != OP_MEM) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	if ((ctxt->d & Sse)
	    && ((ops->get_cr(ctxt, 0) & X86_CR0_EM)
		|| !(ops->get_cr(ctxt, 4) & X86_CR4_OSFXSR))) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	if ((ctxt->d & Sse) && (ops->get_cr(ctxt, 0) & X86_CR0_TS)) {
		rc = emulate_nm(ctxt);
		goto done;
	}

	if (unlikely(ctxt->guest_mode) && ctxt->intercept) {
		rc = emulator_check_intercept(ctxt, ctxt->intercept,
					      X86_ICPT_PRE_EXCEPT);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	 
	if ((ctxt->d & Priv) && ops->cpl(ctxt)) {
		rc = emulate_gp(ctxt, 0);
		goto done;
	}

	 
	if ((ctxt->d & Prot) && !(ctxt->mode & X86EMUL_MODE_PROT)) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	 
	if (ctxt->check_perm) {
		rc = ctxt->check_perm(ctxt);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if (unlikely(ctxt->guest_mode) && ctxt->intercept) {
		rc = emulator_check_intercept(ctxt, ctxt->intercept,
					      X86_ICPT_POST_EXCEPT);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if (ctxt->rep_prefix && (ctxt->d & String)) {
		 
		if (address_mask(ctxt, ctxt->regs[VCPU_REGS_RCX]) == 0) {
			ctxt->eip = ctxt->_eip;
			goto done;
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

	if (unlikely(ctxt->guest_mode) && ctxt->intercept) {
		rc = emulator_check_intercept(ctxt, ctxt->intercept,
					      X86_ICPT_POST_MEMACCESS);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if (ctxt->execute) {
		rc = ctxt->execute(ctxt);
		if (rc != X86EMUL_CONTINUE)
			goto done;
		goto writeback;
	}

	if (ctxt->twobyte)
		goto twobyte_insn;

	switch (ctxt->b) {
	case 0x40 ... 0x47:  
		emulate_1op(ctxt, "inc");
		break;
	case 0x48 ... 0x4f:  
		emulate_1op(ctxt, "dec");
		break;
	case 0x63:		 
		if (ctxt->mode != X86EMUL_MODE_PROT64)
			goto cannot_emulate;
		ctxt->dst.val = (s32) ctxt->src.val;
		break;
	case 0x70 ... 0x7f:  
		if (test_cc(ctxt->b, ctxt->eflags))
			jmp_rel(ctxt, ctxt->src.val);
		break;
	case 0x8d:  
		ctxt->dst.val = ctxt->src.addr.mem.ea;
		break;
	case 0x90 ... 0x97:  
		if (ctxt->dst.addr.reg == &ctxt->regs[VCPU_REGS_RAX])
			break;
		rc = em_xchg(ctxt);
		break;
	case 0x98:  
		switch (ctxt->op_bytes) {
		case 2: ctxt->dst.val = (s8)ctxt->dst.val; break;
		case 4: ctxt->dst.val = (s16)ctxt->dst.val; break;
		case 8: ctxt->dst.val = (s32)ctxt->dst.val; break;
		}
		break;
	case 0xc0 ... 0xc1:
		rc = em_grp2(ctxt);
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
	case 0xd0 ... 0xd1:	 
		rc = em_grp2(ctxt);
		break;
	case 0xd2 ... 0xd3:	 
		ctxt->src.val = ctxt->regs[VCPU_REGS_RCX];
		rc = em_grp2(ctxt);
		break;
	case 0xe9:  
	case 0xeb:  
		jmp_rel(ctxt, ctxt->src.val);
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
	rc = writeback(ctxt);
	if (rc != X86EMUL_CONTINUE)
		goto done;

	 
	ctxt->dst.type = saved_dst_type;

	if ((ctxt->d & SrcMask) == SrcSI)
		string_addr_inc(ctxt, seg_override(ctxt),
				VCPU_REGS_RSI, &ctxt->src);

	if ((ctxt->d & DstMask) == DstDI)
		string_addr_inc(ctxt, VCPU_SREG_ES, VCPU_REGS_RDI,
				&ctxt->dst);

	if (ctxt->rep_prefix && (ctxt->d & String)) {
		struct read_cache *r = &ctxt->io_read;
		register_address_increment(ctxt, &ctxt->regs[VCPU_REGS_RCX], -1);

		if (!string_insn_completed(ctxt)) {
			 
			if ((r->end != 0 || ctxt->regs[VCPU_REGS_RCX] & 0x3ff) &&
			    (r->end == 0 || r->end != r->pos)) {
				 
				ctxt->mem_read.end = 0;
				return EMULATION_RESTART;
			}
			goto done;  
		}
	}

	ctxt->eip = ctxt->_eip;

done:
	if (rc == X86EMUL_PROPAGATE_FAULT)
		ctxt->have_exception = true;
	if (rc == X86EMUL_INTERCEPTED)
		return EMULATION_INTERCEPTED;

	return (rc == X86EMUL_UNHANDLEABLE) ? EMULATION_FAILED : EMULATION_OK;

twobyte_insn:
	switch (ctxt->b) {
	case 0x09:		 
		(ctxt->ops->wbinvd)(ctxt);
		break;
	case 0x08:		 
	case 0x0d:		 
	case 0x18:		 
		break;
	case 0x20:  
		ctxt->dst.val = ops->get_cr(ctxt, ctxt->modrm_reg);
		break;
	case 0x21:  
		ops->get_dr(ctxt, ctxt->modrm_reg, &ctxt->dst.val);
		break;
	case 0x40 ... 0x4f:	 
		ctxt->dst.val = ctxt->dst.orig_val = ctxt->src.val;
		if (!test_cc(ctxt->b, ctxt->eflags))
			ctxt->dst.type = OP_NONE;  
		break;
	case 0x80 ... 0x8f:  
		if (test_cc(ctxt->b, ctxt->eflags))
			jmp_rel(ctxt, ctxt->src.val);
		break;
	case 0x90 ... 0x9f:      
		ctxt->dst.val = test_cc(ctxt->b, ctxt->eflags);
		break;
	case 0xa4:  
	case 0xa5:  
		emulate_2op_cl(ctxt, "shld");
		break;
	case 0xac:  
	case 0xad:  
		emulate_2op_cl(ctxt, "shrd");
		break;
	case 0xae:               
		break;
	case 0xb6 ... 0xb7:	 
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->d & ByteOp) ? (u8) ctxt->src.val
						       : (u16) ctxt->src.val;
		break;
	case 0xbe ... 0xbf:	 
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->d & ByteOp) ? (s8) ctxt->src.val :
							(s16) ctxt->src.val;
		break;
	case 0xc0 ... 0xc1:	 
		emulate_2op_SrcV(ctxt, "add");
		 
		ctxt->src.val = ctxt->dst.orig_val;
		write_register_operand(&ctxt->src);
		break;
	case 0xc3:		 
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->op_bytes == 4) ? (u32) ctxt->src.val :
							(u64) ctxt->src.val;
		break;
	default:
		goto cannot_emulate;
	}

	if (rc != X86EMUL_CONTINUE)
		goto done;

	goto writeback;

cannot_emulate:
	return EMULATION_FAILED;
}
