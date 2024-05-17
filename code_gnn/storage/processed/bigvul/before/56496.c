int sys_rt_sigreturn(unsigned long r3, unsigned long r4, unsigned long r5,
		     unsigned long r6, unsigned long r7, unsigned long r8,
		     struct pt_regs *regs)
{
	struct ucontext __user *uc = (struct ucontext __user *)regs->gpr[1];
	sigset_t set;
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	unsigned long msr;
#endif

	 
	current->restart_block.fn = do_no_restart_syscall;

	if (!access_ok(VERIFY_READ, uc, sizeof(*uc)))
		goto badframe;

	if (__copy_from_user(&set, &uc->uc_sigmask, sizeof(set)))
		goto badframe;
	set_current_blocked(&set);
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	if (__get_user(msr, &uc->uc_mcontext.gp_regs[PT_MSR]))
		goto badframe;
	if (MSR_TM_ACTIVE(msr)) {
		 
		struct ucontext __user *uc_transact;
		if (__get_user(uc_transact, &uc->uc_link))
			goto badframe;
		if (restore_tm_sigcontexts(regs, &uc->uc_mcontext,
					   &uc_transact->uc_mcontext))
			goto badframe;
	}
	else
	 
#endif
	if (restore_sigcontext(regs, NULL, 1, &uc->uc_mcontext))
		goto badframe;

	if (restore_altstack(&uc->uc_stack))
		goto badframe;

	set_thread_flag(TIF_RESTOREALL);
	return 0;

badframe:
	if (show_unhandled_signals)
		printk_ratelimited(regs->msr & MSR_64BIT ? fmt64 : fmt32,
				   current->comm, current->pid, "rt_sigreturn",
				   (long)uc, regs->nip, regs->link);

	force_sig(SIGSEGV, current);
	return 0;
}
