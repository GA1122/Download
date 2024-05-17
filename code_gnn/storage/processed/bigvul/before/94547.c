static int __load_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				     u16 selector, int seg, u8 cpl, bool in_task_switch)
{
	struct desc_struct seg_desc, old_desc;
	u8 dpl, rpl;
	unsigned err_vec = GP_VECTOR;
	u32 err_code = 0;
	bool null_selector = !(selector & ~0x3);  
	ulong desc_addr;
	int ret;
	u16 dummy;
	u32 base3 = 0;

	memset(&seg_desc, 0, sizeof seg_desc);

	if (ctxt->mode == X86EMUL_MODE_REAL) {
		 
		ctxt->ops->get_segment(ctxt, &dummy, &seg_desc, NULL, seg);
		set_desc_base(&seg_desc, selector << 4);
		goto load;
	} else if (seg <= VCPU_SREG_GS && ctxt->mode == X86EMUL_MODE_VM86) {
		 
		set_desc_base(&seg_desc, selector << 4);
		set_desc_limit(&seg_desc, 0xffff);
		seg_desc.type = 3;
		seg_desc.p = 1;
		seg_desc.s = 1;
		seg_desc.dpl = 3;
		goto load;
	}

	rpl = selector & 3;

	 
	if ((seg == VCPU_SREG_CS
	     || (seg == VCPU_SREG_SS
		 && (ctxt->mode != X86EMUL_MODE_PROT64 || rpl != cpl))
	     || seg == VCPU_SREG_TR)
	    && null_selector)
		goto exception;

	 
	if (seg == VCPU_SREG_TR && (selector & (1 << 2)))
		goto exception;

	if (null_selector)  
		goto load;

	ret = read_segment_descriptor(ctxt, selector, &seg_desc, &desc_addr);
	if (ret != X86EMUL_CONTINUE)
		return ret;

	err_code = selector & 0xfffc;
	err_vec = in_task_switch ? TS_VECTOR : GP_VECTOR;

	 
	if (seg <= VCPU_SREG_GS && !seg_desc.s)
		goto exception;

	if (!seg_desc.p) {
		err_vec = (seg == VCPU_SREG_SS) ? SS_VECTOR : NP_VECTOR;
		goto exception;
	}

	dpl = seg_desc.dpl;

	switch (seg) {
	case VCPU_SREG_SS:
		 
		if (rpl != cpl || (seg_desc.type & 0xa) != 0x2 || dpl != cpl)
			goto exception;
		break;
	case VCPU_SREG_CS:
		if (!(seg_desc.type & 8))
			goto exception;

		if (seg_desc.type & 4) {
			 
			if (dpl > cpl)
				goto exception;
		} else {
			 
			if (rpl > cpl || dpl != cpl)
				goto exception;
		}
		 
		if (seg_desc.d && seg_desc.l) {
			u64 efer = 0;

			ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);
			if (efer & EFER_LMA)
				goto exception;
		}

		 
		selector = (selector & 0xfffc) | cpl;
		break;
	case VCPU_SREG_TR:
		if (seg_desc.s || (seg_desc.type != 1 && seg_desc.type != 9))
			goto exception;
		old_desc = seg_desc;
		seg_desc.type |= 2;  
		ret = ctxt->ops->cmpxchg_emulated(ctxt, desc_addr, &old_desc, &seg_desc,
						  sizeof(seg_desc), &ctxt->exception);
		if (ret != X86EMUL_CONTINUE)
			return ret;
		break;
	case VCPU_SREG_LDTR:
		if (seg_desc.s || seg_desc.type != 2)
			goto exception;
		break;
	default:  
		 
		if ((seg_desc.type & 0xa) == 0x8 ||
		    (((seg_desc.type & 0xc) != 0xc) &&
		     (rpl > dpl && cpl > dpl)))
			goto exception;
		break;
	}

	if (seg_desc.s) {
		 
		seg_desc.type |= 1;
		ret = write_segment_descriptor(ctxt, selector, &seg_desc);
		if (ret != X86EMUL_CONTINUE)
			return ret;
	} else if (ctxt->mode == X86EMUL_MODE_PROT64) {
		ret = ctxt->ops->read_std(ctxt, desc_addr+8, &base3,
				sizeof(base3), &ctxt->exception);
		if (ret != X86EMUL_CONTINUE)
			return ret;
	}
load:
	ctxt->ops->set_segment(ctxt, selector, &seg_desc, base3, seg);
	return X86EMUL_CONTINUE;
exception:
	return emulate_exception(ctxt, err_vec, err_code, true);
}