static long restore_user_regs(struct pt_regs *regs,
			      struct mcontext __user *sr, int sig)
{
	long err;
	unsigned int save_r2 = 0;
	unsigned long msr;
#ifdef CONFIG_VSX
	int i;
#endif

	 
	if (!sig)
		save_r2 = (unsigned int)regs->gpr[2];
	err = restore_general_regs(regs, sr);
	regs->trap = 0;
	err |= __get_user(msr, &sr->mc_gregs[PT_MSR]);
	if (!sig)
		regs->gpr[2] = (unsigned long) save_r2;
	if (err)
		return 1;

	 
	if (sig)
		regs->msr = (regs->msr & ~MSR_LE) | (msr & MSR_LE);

	 
	discard_lazy_cpu_state();

#ifdef CONFIG_ALTIVEC
	 
	regs->msr &= ~MSR_VEC;
	if (msr & MSR_VEC) {
		 
		if (__copy_from_user(&current->thread.vr_state, &sr->mc_vregs,
				     sizeof(sr->mc_vregs)))
			return 1;
	} else if (current->thread.used_vr)
		memset(&current->thread.vr_state, 0,
		       ELF_NVRREG * sizeof(vector128));

	 
	if (__get_user(current->thread.vrsave, (u32 __user *)&sr->mc_vregs[32]))
		return 1;
	if (cpu_has_feature(CPU_FTR_ALTIVEC))
		mtspr(SPRN_VRSAVE, current->thread.vrsave);
#endif  
	if (copy_fpr_from_user(current, &sr->mc_fregs))
		return 1;

#ifdef CONFIG_VSX
	 
	regs->msr &= ~MSR_VSX;
	if (msr & MSR_VSX) {
		 
		if (copy_vsx_from_user(current, &sr->mc_vsregs))
			return 1;
	} else if (current->thread.used_vsr)
		for (i = 0; i < 32 ; i++)
			current->thread.fp_state.fpr[i][TS_VSRLOWOFFSET] = 0;
#endif  
	 
	regs->msr &= ~(MSR_FP | MSR_FE0 | MSR_FE1);

#ifdef CONFIG_SPE
	 
	regs->msr &= ~MSR_SPE;
	if (msr & MSR_SPE) {
		 
		if (__copy_from_user(current->thread.evr, &sr->mc_vregs,
				     ELF_NEVRREG * sizeof(u32)))
			return 1;
	} else if (current->thread.used_spe)
		memset(current->thread.evr, 0, ELF_NEVRREG * sizeof(u32));

	 
	if (__get_user(current->thread.spefscr, (u32 __user *)&sr->mc_vregs + ELF_NEVRREG))
		return 1;
#endif  

	return 0;
}
