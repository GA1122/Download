void update_cr_regs(struct task_struct *task)
{
	struct pt_regs *regs = task_pt_regs(task);
	struct thread_struct *thread = &task->thread;
	struct per_regs old, new;

#ifdef CONFIG_64BIT
	 
	if (MACHINE_HAS_TE) {
		unsigned long cr, cr_new;

		__ctl_store(cr, 0, 0);
		 
		cr_new = cr | (1UL << 55);
		if (task->thread.per_flags & PER_FLAG_NO_TE)
			cr_new &= ~(1UL << 55);
		if (cr_new != cr)
			__ctl_load(cr_new, 0, 0);
		 
		__ctl_store(cr, 2, 2);
		cr_new = cr & ~3UL;
		if (task->thread.per_flags & PER_FLAG_TE_ABORT_RAND) {
			if (task->thread.per_flags & PER_FLAG_TE_ABORT_RAND_TEND)
				cr_new |= 1UL;
			else
				cr_new |= 2UL;
		}
		if (cr_new != cr)
			__ctl_load(cr_new, 2, 2);
	}
#endif
	 
	new.control = thread->per_user.control;
	new.start = thread->per_user.start;
	new.end = thread->per_user.end;

	 
	if (test_tsk_thread_flag(task, TIF_SINGLE_STEP)) {
		if (test_tsk_thread_flag(task, TIF_BLOCK_STEP))
			new.control |= PER_EVENT_BRANCH;
		else
			new.control |= PER_EVENT_IFETCH;
#ifdef CONFIG_64BIT
		new.control |= PER_CONTROL_SUSPENSION;
		new.control |= PER_EVENT_TRANSACTION_END;
#endif
		new.start = 0;
		new.end = PSW_ADDR_INSN;
	}

	 
	if (!(new.control & PER_EVENT_MASK)) {
		regs->psw.mask &= ~PSW_MASK_PER;
		return;
	}
	regs->psw.mask |= PSW_MASK_PER;
	__ctl_store(old, 9, 11);
	if (memcmp(&new, &old, sizeof(struct per_regs)) != 0)
		__ctl_load(new, 9, 11);
}