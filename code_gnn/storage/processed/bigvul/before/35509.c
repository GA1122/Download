static int check_svme_pa(struct x86_emulate_ctxt *ctxt)
{
	u64 rax = reg_read(ctxt, VCPU_REGS_RAX);

	 
	if (rax & 0xffff000000000000ULL)
		return emulate_gp(ctxt, 0);

	return check_svme(ctxt);
}