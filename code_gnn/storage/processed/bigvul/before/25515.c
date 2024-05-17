static noinline void do_sigsegv(struct pt_regs *regs, long int_code,
				int si_code, unsigned long trans_exc_code)
{
	struct siginfo si;
	unsigned long address;

	address = trans_exc_code & __FAIL_ADDR_MASK;
	current->thread.prot_addr = address;
	current->thread.trap_no = int_code;
	report_user_fault(regs, int_code, SIGSEGV, address);
	si.si_signo = SIGSEGV;
	si.si_code = si_code;
	si.si_addr = (void __user *) address;
	force_sig_info(SIGSEGV, &si, current);
}
