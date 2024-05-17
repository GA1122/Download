static long setup_sigcontext(struct sigcontext __user *sc, struct pt_regs *regs,
		 int signr, sigset_t *set, unsigned long handler,
		 int ctx_has_vsx_region)
{
	 
#ifdef CONFIG_ALTIVEC
	elf_vrreg_t __user *v_regs = sigcontext_vmx_regs(sc);
#endif
	unsigned long msr = regs->msr;
	long err = 0;

#ifdef CONFIG_ALTIVEC
	err |= __put_user(v_regs, &sc->v_regs);

	 
	if (current->thread.used_vr) {
		flush_altivec_to_thread(current);
		 
		err |= __copy_to_user(v_regs, &current->thread.vr_state,
				      33 * sizeof(vector128));
		 
		msr |= MSR_VEC;
	}
	 
	if (cpu_has_feature(CPU_FTR_ALTIVEC))
		current->thread.vrsave = mfspr(SPRN_VRSAVE);
	err |= __put_user(current->thread.vrsave, (u32 __user *)&v_regs[33]);
#else  
	err |= __put_user(0, &sc->v_regs);
#endif  
	flush_fp_to_thread(current);
	 
	err |= copy_fpr_to_user(&sc->fp_regs, current);

	 
	msr &= ~MSR_VSX;
#ifdef CONFIG_VSX
	 
	if (current->thread.used_vsr && ctx_has_vsx_region) {
		__giveup_vsx(current);
		v_regs += ELF_NVRREG;
		err |= copy_vsx_to_user(v_regs, current);
		 
		msr |= MSR_VSX;
	}
#endif  
	err |= __put_user(&sc->gp_regs, &sc->regs);
	WARN_ON(!FULL_REGS(regs));
	err |= __copy_to_user(&sc->gp_regs, regs, GP_REGS_SIZE);
	err |= __put_user(msr, &sc->gp_regs[PT_MSR]);
	err |= __put_user(signr, &sc->signal);
	err |= __put_user(handler, &sc->handler);
	if (set != NULL)
		err |=  __put_user(set->sig[0], &sc->oldmask);

	return err;
}
