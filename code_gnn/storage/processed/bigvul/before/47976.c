static void string_registers_quirk(struct x86_emulate_ctxt *ctxt)
{
	 
#ifdef CONFIG_X86_64
	if (ctxt->ad_bytes != 4 || !vendor_intel(ctxt))
		return;

	*reg_write(ctxt, VCPU_REGS_RCX) = 0;

	switch (ctxt->b) {
	case 0xa4:	 
	case 0xa5:	 
		*reg_rmw(ctxt, VCPU_REGS_RSI) &= (u32)-1;
		 
	case 0xaa:	 
	case 0xab:	 
		*reg_rmw(ctxt, VCPU_REGS_RDI) &= (u32)-1;
	}
#endif
}
