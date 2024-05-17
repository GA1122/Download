int handle_signal32(struct ksignal *ksig, sigset_t *oldset, struct pt_regs *regs)
{
	struct sigcontext __user *sc;
	struct sigframe __user *frame;
	struct mcontext __user *tm_mctx = NULL;
	unsigned long newsp = 0;
	int sigret;
	unsigned long tramp;

	 
	frame = get_sigframe(ksig, get_tm_stackpointer(regs), sizeof(*frame), 1);
	if (unlikely(frame == NULL))
		goto badframe;
	sc = (struct sigcontext __user *) &frame->sctx;

#if _NSIG != 64
#error "Please adjust handle_signal()"
#endif
	if (__put_user(to_user_ptr(ksig->ka.sa.sa_handler), &sc->handler)
	    || __put_user(oldset->sig[0], &sc->oldmask)
#ifdef CONFIG_PPC64
	    || __put_user((oldset->sig[0] >> 32), &sc->_unused[3])
#else
	    || __put_user(oldset->sig[1], &sc->_unused[3])
#endif
	    || __put_user(to_user_ptr(&frame->mctx), &sc->regs)
	    || __put_user(ksig->sig, &sc->signal))
		goto badframe;

	if (vdso32_sigtramp && current->mm->context.vdso_base) {
		sigret = 0;
		tramp = current->mm->context.vdso_base + vdso32_sigtramp;
	} else {
		sigret = __NR_sigreturn;
		tramp = (unsigned long) frame->mctx.tramp;
	}

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	tm_mctx = &frame->mctx_transact;
	if (MSR_TM_ACTIVE(regs->msr)) {
		if (save_tm_user_regs(regs, &frame->mctx, &frame->mctx_transact,
				      sigret))
			goto badframe;
	}
	else
#endif
	{
		if (save_user_regs(regs, &frame->mctx, tm_mctx, sigret, 1))
			goto badframe;
	}

	regs->link = tramp;

	current->thread.fp_state.fpscr = 0;	 

	 
	newsp = ((unsigned long)frame) - __SIGNAL_FRAMESIZE;
	if (put_user(regs->gpr[1], (u32 __user *)newsp))
		goto badframe;

	regs->gpr[1] = newsp;
	regs->gpr[3] = ksig->sig;
	regs->gpr[4] = (unsigned long) sc;
	regs->nip = (unsigned long) (unsigned long)ksig->ka.sa.sa_handler;
	 
	regs->msr &= ~MSR_LE;
	return 0;

badframe:
	if (show_unhandled_signals)
		printk_ratelimited(KERN_INFO
				   "%s[%d]: bad frame in handle_signal32: "
				   "%p nip %08lx lr %08lx\n",
				   current->comm, current->pid,
				   frame, regs->nip, regs->link);

	return 1;
}