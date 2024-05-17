long sys_sigreturn(int r3, int r4, int r5, int r6, int r7, int r8,
		       struct pt_regs *regs)
{
	struct sigframe __user *sf;
	struct sigcontext __user *sc;
	struct sigcontext sigctx;
	struct mcontext __user *sr;
	void __user *addr;
	sigset_t set;
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	struct mcontext __user *mcp, *tm_mcp;
	unsigned long msr_hi;
#endif

	 
	current->restart_block.fn = do_no_restart_syscall;

	sf = (struct sigframe __user *)(regs->gpr[1] + __SIGNAL_FRAMESIZE);
	sc = &sf->sctx;
	addr = sc;
	if (copy_from_user(&sigctx, sc, sizeof(sigctx)))
		goto badframe;

#ifdef CONFIG_PPC64
	 
	set.sig[0] = sigctx.oldmask + ((long)(sigctx._unused[3]) << 32);
#else
	set.sig[0] = sigctx.oldmask;
	set.sig[1] = sigctx._unused[3];
#endif
	set_current_blocked(&set);

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	mcp = (struct mcontext __user *)&sf->mctx;
	tm_mcp = (struct mcontext __user *)&sf->mctx_transact;
	if (__get_user(msr_hi, &tm_mcp->mc_gregs[PT_MSR]))
		goto badframe;
	if (MSR_TM_ACTIVE(msr_hi<<32)) {
		if (!cpu_has_feature(CPU_FTR_TM))
			goto badframe;
		if (restore_tm_user_regs(regs, mcp, tm_mcp))
			goto badframe;
	} else
#endif
	{
		sr = (struct mcontext __user *)from_user_ptr(sigctx.regs);
		addr = sr;
		if (!access_ok(VERIFY_READ, sr, sizeof(*sr))
		    || restore_user_regs(regs, sr, 1))
			goto badframe;
	}

	set_thread_flag(TIF_RESTOREALL);
	return 0;

badframe:
	if (show_unhandled_signals)
		printk_ratelimited(KERN_INFO
				   "%s[%d]: bad frame in sys_sigreturn: "
				   "%p nip %08lx lr %08lx\n",
				   current->comm, current->pid,
				   addr, regs->nip, regs->link);

	force_sig(SIGSEGV, current);
	return 0;
}