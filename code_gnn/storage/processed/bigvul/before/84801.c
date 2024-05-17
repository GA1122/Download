static int ptrace_set_debugreg(struct task_struct *task, unsigned long addr,
			       unsigned long data)
{
#ifdef CONFIG_HAVE_HW_BREAKPOINT
	int ret;
	struct thread_struct *thread = &(task->thread);
	struct perf_event *bp;
	struct perf_event_attr attr;
#endif  
#ifndef CONFIG_PPC_ADV_DEBUG_REGS
	struct arch_hw_breakpoint hw_brk;
#endif

	 
	if (addr > 0)
		return -EINVAL;

	 
	if ((data & ~0x7UL) >= TASK_SIZE)
		return -EIO;

#ifndef CONFIG_PPC_ADV_DEBUG_REGS
	 

	 
	if (data && !(data & HW_BRK_TYPE_TRANSLATE))
		return -EIO;
	hw_brk.address = data & (~HW_BRK_TYPE_DABR);
	hw_brk.type = (data & HW_BRK_TYPE_DABR) | HW_BRK_TYPE_PRIV_ALL;
	hw_brk.len = 8;
#ifdef CONFIG_HAVE_HW_BREAKPOINT
	bp = thread->ptrace_bps[0];
	if ((!data) || !(hw_brk.type & HW_BRK_TYPE_RDWR)) {
		if (bp) {
			unregister_hw_breakpoint(bp);
			thread->ptrace_bps[0] = NULL;
		}
		return 0;
	}
	if (bp) {
		attr = bp->attr;
		attr.bp_addr = hw_brk.address;
		arch_bp_generic_fields(hw_brk.type, &attr.bp_type);

		 
		attr.disabled = false;

		ret =  modify_user_hw_breakpoint(bp, &attr);
		if (ret) {
			return ret;
		}
		thread->ptrace_bps[0] = bp;
		thread->hw_brk = hw_brk;
		return 0;
	}

	 
	hw_breakpoint_init(&attr);
	attr.bp_addr = hw_brk.address;
	arch_bp_generic_fields(hw_brk.type,
			       &attr.bp_type);

	thread->ptrace_bps[0] = bp = register_user_hw_breakpoint(&attr,
					       ptrace_triggered, NULL, task);
	if (IS_ERR(bp)) {
		thread->ptrace_bps[0] = NULL;
		return PTR_ERR(bp);
	}

#endif  
	task->thread.hw_brk = hw_brk;
#else  
	 

	 
	task->thread.debug.dac1 = data & ~0x3UL;

	if (task->thread.debug.dac1 == 0) {
		dbcr_dac(task) &= ~(DBCR_DAC1R | DBCR_DAC1W);
		if (!DBCR_ACTIVE_EVENTS(task->thread.debug.dbcr0,
					task->thread.debug.dbcr1)) {
			task->thread.regs->msr &= ~MSR_DE;
			task->thread.debug.dbcr0 &= ~DBCR0_IDM;
		}
		return 0;
	}

	 

	if (!(data & 0x3UL))
		return -EINVAL;

	 
	task->thread.debug.dbcr0 |= DBCR0_IDM;

	 
	dbcr_dac(task) &= ~(DBCR_DAC1R|DBCR_DAC1W);
	if (data & 0x1UL)
		dbcr_dac(task) |= DBCR_DAC1R;
	if (data & 0x2UL)
		dbcr_dac(task) |= DBCR_DAC1W;
	task->thread.regs->msr |= MSR_DE;
#endif  
	return 0;
}
