void CacheLockingException(struct pt_regs *regs, unsigned long address,
			   unsigned long error_code)
{
	 
	if (error_code & (ESR_DLK|ESR_ILK))
		_exception(SIGILL, regs, ILL_PRVOPC, regs->nip);
	return;
}
