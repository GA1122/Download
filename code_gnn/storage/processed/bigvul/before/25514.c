static noinline void do_sigbus(struct pt_regs *regs, long int_code,
			       unsigned long trans_exc_code)
{
	struct task_struct *tsk = current;
	unsigned long address;
	struct siginfo si;

	 
	address = trans_exc_code & __FAIL_ADDR_MASK;
	tsk->thread.prot_addr = address;
	tsk->thread.trap_no = int_code;
	si.si_signo = SIGBUS;
	si.si_errno = 0;
	si.si_code = BUS_ADRERR;
	si.si_addr = (void __user *) address;
	force_sig_info(SIGBUS, &si, tsk);
}
