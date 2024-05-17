static long setup_tm_sigcontexts(struct sigcontext __user *sc,
				 struct sigcontext __user *tm_sc,
				 struct pt_regs *regs,
				 int signr, sigset_t *set, unsigned long handler)
{
	 
#ifdef CONFIG_ALTIVEC
	elf_vrreg_t __user *v_regs = sigcontext_vmx_regs(sc);
	elf_vrreg_t __user *tm_v_regs = sigcontext_vmx_regs(tm_sc);
#endif
	unsigned long msr = regs->msr;
	long err = 0;

	BUG_ON(!MSR_TM_ACTIVE(regs->msr));

	 
	regs->msr &= ~MSR_TS_MASK;

	flush_fp_to_thread(current);

#ifdef CONFIG_ALTIVEC
	err |= __put_user(v_regs, &sc->v_regs);
	err |= __put_user(tm_v_regs, &tm_sc->v_regs);

	 
	if (current->thread.used_vr) {
		flush_altivec_to_thread(current);
		 
		err |= __copy_to_user(v_regs, &current->thread.vr_state,
				      33 * sizeof(vector128));
		 
		if (msr & MSR_VEC)
			err |= __copy_to_user(tm_v_regs,
					      &current->thread.transact_vr,
					      33 * sizeof(vector128));
		else
			err |= __copy_to_user(tm_v_regs,
					      &current->thread.vr_state,
					      33 * sizeof(vector128));

		 
		msr |= MSR_VEC;
	}
	 
	if (cpu_has_feature(CPU_FTR_ALTIVEC))
		current->thread.vrsave = mfspr(SPRN_VRSAVE);
	err |= __put_user(current->thread.vrsave, (u32 __user *)&v_regs[33]);
	if (msr & MSR_VEC)
		err |= __put_user(current->thread.transact_vrsave,
				  (u32 __user *)&tm_v_regs[33]);
	else
		err |= __put_user(current->thread.vrsave,
				  (u32 __user *)&tm_v_regs[33]);

#else  
	err |= __put_user(0, &sc->v_regs);
	err |= __put_user(0, &tm_sc->v_regs);
#endif  

	 
	err |= copy_fpr_to_user(&sc->fp_regs, current);
	if (msr & MSR_FP)
		err |= copy_transact_fpr_to_user(&tm_sc->fp_regs, current);
	else
		err |= copy_fpr_to_user(&tm_sc->fp_regs, current);

#ifdef CONFIG_VSX
	 
	if (current->thread.used_vsr) {
		__giveup_vsx(current);
		v_regs += ELF_NVRREG;
		tm_v_regs += ELF_NVRREG;

		err |= copy_vsx_to_user(v_regs, current);

		if (msr & MSR_VSX)
			err |= copy_transact_vsx_to_user(tm_v_regs, current);
		else
			err |= copy_vsx_to_user(tm_v_regs, current);

		 
		msr |= MSR_VSX;
	}
#endif  

	err |= __put_user(&sc->gp_regs, &sc->regs);
	err |= __put_user(&tm_sc->gp_regs, &tm_sc->regs);
	WARN_ON(!FULL_REGS(regs));
	err |= __copy_to_user(&tm_sc->gp_regs, regs, GP_REGS_SIZE);
	err |= __copy_to_user(&sc->gp_regs,
			      &current->thread.ckpt_regs, GP_REGS_SIZE);
	err |= __put_user(msr, &tm_sc->gp_regs[PT_MSR]);
	err |= __put_user(msr, &sc->gp_regs[PT_MSR]);
	err |= __put_user(signr, &sc->signal);
	err |= __put_user(handler, &sc->handler);
	if (set != NULL)
		err |=  __put_user(set->sig[0], &sc->oldmask);

	return err;
}
