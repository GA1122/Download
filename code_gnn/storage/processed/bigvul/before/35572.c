static int fastop(struct x86_emulate_ctxt *ctxt, void (*fop)(struct fastop *))
{
	ulong flags = (ctxt->eflags & EFLAGS_MASK) | X86_EFLAGS_IF;
	if (!(ctxt->d & ByteOp))
		fop += __ffs(ctxt->dst.bytes) * FASTOP_SIZE;
	asm("push %[flags]; popf; call *%[fastop]; pushf; pop %[flags]\n"
	    : "+a"(ctxt->dst.val), "+d"(ctxt->src.val), [flags]"+D"(flags),
	      [fastop]"+S"(fop)
	    : "c"(ctxt->src2.val));
	ctxt->eflags = (ctxt->eflags & ~EFLAGS_MASK) | (flags & EFLAGS_MASK);
	if (!fop)  
		return emulate_de(ctxt);
	return X86EMUL_CONTINUE;
}