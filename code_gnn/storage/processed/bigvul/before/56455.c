static inline void tm_recheckpoint_new_task(struct task_struct *new)
{
	unsigned long msr;

	if (!cpu_has_feature(CPU_FTR_TM))
		return;

	 
	if (!new->thread.regs)
		return;

	if (!MSR_TM_ACTIVE(new->thread.regs->msr)){
		tm_restore_sprs(&new->thread);
		return;
	}
	msr = new->thread.ckpt_regs.msr;
	 
	TM_DEBUG("*** tm_recheckpoint of pid %d "
		 "(new->msr 0x%lx, new->origmsr 0x%lx)\n",
		 new->pid, new->thread.regs->msr, msr);

	 
	tm_recheckpoint(&new->thread, msr);

	 
	if (msr & MSR_FP) {
		do_load_up_transact_fpu(&new->thread);
		new->thread.regs->msr |=
			(MSR_FP | new->thread.fpexc_mode);
	}
#ifdef CONFIG_ALTIVEC
	if (msr & MSR_VEC) {
		do_load_up_transact_altivec(&new->thread);
		new->thread.regs->msr |= MSR_VEC;
	}
#endif
	 
	if (msr & MSR_VSX)
		new->thread.regs->msr |= MSR_VSX;

	TM_DEBUG("*** tm_recheckpoint of pid %d complete "
		 "(kernel msr 0x%lx)\n",
		 new->pid, mfmsr());
}
