int handle_rt_signal32(struct ksignal *ksig, sigset_t *oldset,
		       struct pt_regs *regs)
{
	struct rt_sigframe __user *rt_sf;
	struct mcontext __user *frame;
	struct mcontext __user *tm_frame = NULL;
	void __user *addr;
	unsigned long newsp = 0;
	int sigret;
	unsigned long tramp;

	 
	 
	rt_sf = get_sigframe(ksig, get_tm_stackpointer(regs), sizeof(*rt_sf), 1);
	addr = rt_sf;
	if (unlikely(rt_sf == NULL))
		goto badframe;

	 
	if (copy_siginfo_to_user(&rt_sf->info, &ksig->info)
	    || __put_user(0, &rt_sf->uc.uc_flags)
	    || __save_altstack(&rt_sf->uc.uc_stack, regs->gpr[1])
	    || __put_user(to_user_ptr(&rt_sf->uc.uc_mcontext),
		    &rt_sf->uc.uc_regs)
	    || put_sigset_t(&rt_sf->uc.uc_sigmask, oldset))
		goto badframe;

	 
	frame = &rt_sf->uc.uc_mcontext;
	addr = frame;
	if (vdso32_rt_sigtramp && current->mm->context.vdso_base) {
		sigret = 0;
		tramp = current->mm->context.vdso_base + vdso32_rt_sigtramp;
	} else {
		sigret = __NR_rt_sigreturn;
		tramp = (unsigned long) frame->tramp;
	}

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	tm_frame = &rt_sf->uc_transact.uc_mcontext;
	if (MSR_TM_ACTIVE(regs->msr)) {
		if (__put_user((unsigned long)&rt_sf->uc_transact,
			       &rt_sf->uc.uc_link) ||
		    __put_user((unsigned long)tm_frame,
			       &rt_sf->uc_transact.uc_regs))
			goto badframe;
		if (save_tm_user_regs(regs, frame, tm_frame, sigret))
			goto badframe;
	}
	else
#endif
	{
		if (__put_user(0, &rt_sf->uc.uc_link))
			goto badframe;
		if (save_user_regs(regs, frame, tm_frame, sigret, 1))
			goto badframe;
	}
	regs->link = tramp;

	current->thread.fp_state.fpscr = 0;	 

	 
	newsp = ((unsigned long)rt_sf) - (__SIGNAL_FRAMESIZE + 16);
	addr = (void __user *)regs->gpr[1];
	if (put_user(regs->gpr[1], (u32 __user *)newsp))
		goto badframe;

	 
	regs->gpr[1] = newsp;
	regs->gpr[3] = ksig->sig;
	regs->gpr[4] = (unsigned long) &rt_sf->info;
	regs->gpr[5] = (unsigned long) &rt_sf->uc;
	regs->gpr[6] = (unsigned long) rt_sf;
	regs->nip = (unsigned long) ksig->ka.sa.sa_handler;
	 
	regs->msr &= ~MSR_LE;
	regs->msr |= (MSR_KERNEL & MSR_LE);
	return 0;

badframe:
	if (show_unhandled_signals)
		printk_ratelimited(KERN_INFO
				   "%s[%d]: bad frame in handle_rt_signal32: "
				   "%p nip %08lx lr %08lx\n",
				   current->comm, current->pid,
				   addr, regs->nip, regs->link);

	return 1;
}