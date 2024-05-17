long sys_rt_sigreturn(int r3, int r4, int r5, int r6, int r7, int r8,
		     struct pt_regs *regs)
{
	struct rt_sigframe __user *rt_sf;
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	struct ucontext __user *uc_transact;
	unsigned long msr_hi;
	unsigned long tmp;
	int tm_restore = 0;
#endif
	 
	current->restart_block.fn = do_no_restart_syscall;

	rt_sf = (struct rt_sigframe __user *)
		(regs->gpr[1] + __SIGNAL_FRAMESIZE + 16);
	if (!access_ok(VERIFY_READ, rt_sf, sizeof(*rt_sf)))
		goto bad;
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	if (__get_user(tmp, &rt_sf->uc.uc_link))
		goto bad;
	uc_transact = (struct ucontext __user *)(uintptr_t)tmp;
	if (uc_transact) {
		u32 cmcp;
		struct mcontext __user *mcp;

		if (__get_user(cmcp, &uc_transact->uc_regs))
			return -EFAULT;
		mcp = (struct mcontext __user *)(u64)cmcp;
		 
		if (__get_user(msr_hi, &mcp->mc_gregs[PT_MSR]))
			goto bad;

		if (MSR_TM_ACTIVE(msr_hi<<32)) {
			 
			tm_restore = 1;
			if (do_setcontext_tm(&rt_sf->uc, uc_transact, regs))
				goto bad;
		}
	}
	if (!tm_restore)
		 
#endif
	if (do_setcontext(&rt_sf->uc, regs, 1))
		goto bad;

	 
#ifdef CONFIG_PPC64
	if (compat_restore_altstack(&rt_sf->uc.uc_stack))
		goto bad;
#else
	if (restore_altstack(&rt_sf->uc.uc_stack))
		goto bad;
#endif
	set_thread_flag(TIF_RESTOREALL);
	return 0;

 bad:
	if (show_unhandled_signals)
		printk_ratelimited(KERN_INFO
				   "%s[%d]: bad frame in sys_rt_sigreturn: "
				   "%p nip %08lx lr %08lx\n",
				   current->comm, current->pid,
				   rt_sf, regs->nip, regs->link);

	force_sig(SIGSEGV, current);
	return 0;
}
