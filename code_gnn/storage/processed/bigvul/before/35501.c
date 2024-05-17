static int __linearize(struct x86_emulate_ctxt *ctxt,
		     struct segmented_address addr,
		     unsigned size, bool write, bool fetch,
		     ulong *linear)
{
	struct desc_struct desc;
	bool usable;
	ulong la;
	u32 lim;
	u16 sel;
	unsigned cpl;

	la = seg_base(ctxt, addr.seg) + addr.ea;
	switch (ctxt->mode) {
	case X86EMUL_MODE_PROT64:
		if (((signed long)la << 16) >> 16 != la)
			return emulate_gp(ctxt, 0);
		break;
	default:
		usable = ctxt->ops->get_segment(ctxt, &sel, &desc, NULL,
						addr.seg);
		if (!usable)
			goto bad;
		 
		if ((((ctxt->mode != X86EMUL_MODE_REAL) && (desc.type & 8))
					|| !(desc.type & 2)) && write)
			goto bad;
		 
		if (!fetch && (desc.type & 8) && !(desc.type & 2))
			goto bad;
		lim = desc_limit_scaled(&desc);
		if ((ctxt->mode == X86EMUL_MODE_REAL) && !fetch &&
		    (ctxt->d & NoBigReal)) {
			 
			if (la > 0xffff || (u32)(la + size - 1) > 0xffff)
				goto bad;
		} else if ((desc.type & 8) || !(desc.type & 4)) {
			 
			if (addr.ea > lim || (u32)(addr.ea + size - 1) > lim)
				goto bad;
		} else {
			 
			if (addr.ea <= lim || (u32)(addr.ea + size - 1) <= lim)
				goto bad;
			lim = desc.d ? 0xffffffff : 0xffff;
			if (addr.ea > lim || (u32)(addr.ea + size - 1) > lim)
				goto bad;
		}
		cpl = ctxt->ops->cpl(ctxt);
		if (!(desc.type & 8)) {
			 
			if (cpl > desc.dpl)
				goto bad;
		} else if ((desc.type & 8) && !(desc.type & 4)) {
			 
			if (cpl != desc.dpl)
				goto bad;
		} else if ((desc.type & 8) && (desc.type & 4)) {
			 
			if (cpl < desc.dpl)
				goto bad;
		}
		break;
	}
	if (fetch ? ctxt->mode != X86EMUL_MODE_PROT64 : ctxt->ad_bytes != 8)
		la &= (u32)-1;
	if (insn_aligned(ctxt, size) && ((la & (size - 1)) != 0))
		return emulate_gp(ctxt, 0);
	*linear = la;
	return X86EMUL_CONTINUE;
bad:
	if (addr.seg == VCPU_SREG_SS)
		return emulate_ss(ctxt, sel);
	else
		return emulate_gp(ctxt, sel);
}
