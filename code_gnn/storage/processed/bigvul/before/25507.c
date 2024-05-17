int __handle_fault(unsigned long uaddr, unsigned long pgm_int_code, int write)
{
	struct pt_regs regs;
	int access, fault;

	regs.psw.mask = psw_kernel_bits;
	if (!irqs_disabled())
		regs.psw.mask |= PSW_MASK_IO | PSW_MASK_EXT;
	regs.psw.addr = (unsigned long) __builtin_return_address(0);
	regs.psw.addr |= PSW_ADDR_AMODE;
	uaddr &= PAGE_MASK;
	access = write ? VM_WRITE : VM_READ;
	fault = do_exception(&regs, access, uaddr | 2);
	if (unlikely(fault)) {
		if (fault & VM_FAULT_OOM)
			return -EFAULT;
		else if (fault & VM_FAULT_SIGBUS)
			do_sigbus(&regs, pgm_int_code, uaddr);
	}
	return fault ? -EFAULT : 0;
}
