static void init_decode_cache(struct x86_emulate_ctxt *ctxt,
			      const unsigned long *regs)
{
	memset(&ctxt->twobyte, 0,
	       (void *)&ctxt->regs - (void *)&ctxt->twobyte);
	memcpy(ctxt->regs, regs, sizeof(ctxt->regs));

	ctxt->fetch.start = 0;
	ctxt->fetch.end = 0;
	ctxt->io_read.pos = 0;
	ctxt->io_read.end = 0;
	ctxt->mem_read.pos = 0;
	ctxt->mem_read.end = 0;
}
