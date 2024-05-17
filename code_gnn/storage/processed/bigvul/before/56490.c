int handle_rt_signal64(struct ksignal *ksig, sigset_t *set, struct pt_regs *regs)
{
	struct rt_sigframe __user *frame;
	unsigned long newsp = 0;
	long err = 0;

	frame = get_sigframe(ksig, get_tm_stackpointer(regs), sizeof(*frame), 0);
	if (unlikely(frame == NULL))
		goto badframe;

	err |= __put_user(&frame->info, &frame->pinfo);
	err |= __put_user(&frame->uc, &frame->puc);
	err |= copy_siginfo_to_user(&frame->info, &ksig->info);
	if (err)
		goto badframe;

	 
	err |= __put_user(0, &frame->uc.uc_flags);
	err |= __save_altstack(&frame->uc.uc_stack, regs->gpr[1]);
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	if (MSR_TM_ACTIVE(regs->msr)) {
		 
		err |= __put_user(&frame->uc_transact, &frame->uc.uc_link);
		err |= setup_tm_sigcontexts(&frame->uc.uc_mcontext,
					    &frame->uc_transact.uc_mcontext,
					    regs, ksig->sig,
					    NULL,
					    (unsigned long)ksig->ka.sa.sa_handler);
	} else
#endif
	{
		err |= __put_user(0, &frame->uc.uc_link);
		err |= setup_sigcontext(&frame->uc.uc_mcontext, regs, ksig->sig,
					NULL, (unsigned long)ksig->ka.sa.sa_handler,
					1);
	}
	err |= __copy_to_user(&frame->uc.uc_sigmask, set, sizeof(*set));
	if (err)
		goto badframe;

	 
	current->thread.fp_state.fpscr = 0;

	 
	if (vdso64_rt_sigtramp && current->mm->context.vdso_base) {
		regs->link = current->mm->context.vdso_base + vdso64_rt_sigtramp;
	} else {
		err |= setup_trampoline(__NR_rt_sigreturn, &frame->tramp[0]);
		if (err)
			goto badframe;
		regs->link = (unsigned long) &frame->tramp[0];
	}

	 
	newsp = ((unsigned long)frame) - __SIGNAL_FRAMESIZE;
	err |= put_user(regs->gpr[1], (unsigned long __user *)newsp);

	 
	if (is_elf2_task()) {
		regs->nip = (unsigned long) ksig->ka.sa.sa_handler;
		regs->gpr[12] = regs->nip;
	} else {
		 
		func_descr_t __user *funct_desc_ptr =
			(func_descr_t __user *) ksig->ka.sa.sa_handler;

		err |= get_user(regs->nip, &funct_desc_ptr->entry);
		err |= get_user(regs->gpr[2], &funct_desc_ptr->toc);
	}

	 
	regs->msr &= ~MSR_LE;
	regs->msr |= (MSR_KERNEL & MSR_LE);
	regs->gpr[1] = newsp;
	regs->gpr[3] = ksig->sig;
	regs->result = 0;
	if (ksig->ka.sa.sa_flags & SA_SIGINFO) {
		err |= get_user(regs->gpr[4], (unsigned long __user *)&frame->pinfo);
		err |= get_user(regs->gpr[5], (unsigned long __user *)&frame->puc);
		regs->gpr[6] = (unsigned long) frame;
	} else {
		regs->gpr[4] = (unsigned long)&frame->uc.uc_mcontext;
	}
	if (err)
		goto badframe;

	return 0;

badframe:
	if (show_unhandled_signals)
		printk_ratelimited(regs->msr & MSR_64BIT ? fmt64 : fmt32,
				   current->comm, current->pid, "setup_rt_frame",
				   (long)frame, regs->nip, regs->link);

	return 1;
}