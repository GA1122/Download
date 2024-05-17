static long restore_sigcontext(struct pt_regs *regs, sigset_t *set, int sig,
			      struct sigcontext __user *sc)
{
#ifdef CONFIG_ALTIVEC
	elf_vrreg_t __user *v_regs;
#endif
	unsigned long err = 0;
	unsigned long save_r13 = 0;
	unsigned long msr;
#ifdef CONFIG_VSX
	int i;
#endif

	 
	if (!sig)
		save_r13 = regs->gpr[13];

	 
	err |= __copy_from_user(regs->gpr, sc->gp_regs, sizeof(regs->gpr));
	err |= __get_user(regs->nip, &sc->gp_regs[PT_NIP]);
	 
	err |= __get_user(msr, &sc->gp_regs[PT_MSR]);
	if (sig)
		regs->msr = (regs->msr & ~MSR_LE) | (msr & MSR_LE);
	err |= __get_user(regs->orig_gpr3, &sc->gp_regs[PT_ORIG_R3]);
	err |= __get_user(regs->ctr, &sc->gp_regs[PT_CTR]);
	err |= __get_user(regs->link, &sc->gp_regs[PT_LNK]);
	err |= __get_user(regs->xer, &sc->gp_regs[PT_XER]);
	err |= __get_user(regs->ccr, &sc->gp_regs[PT_CCR]);
	 
	regs->trap = 0;
	err |= __get_user(regs->dar, &sc->gp_regs[PT_DAR]);
	err |= __get_user(regs->dsisr, &sc->gp_regs[PT_DSISR]);
	err |= __get_user(regs->result, &sc->gp_regs[PT_RESULT]);

	if (!sig)
		regs->gpr[13] = save_r13;
	if (set != NULL)
		err |=  __get_user(set->sig[0], &sc->oldmask);

	 
	discard_lazy_cpu_state();

	 
	regs->msr &= ~(MSR_FP | MSR_FE0 | MSR_FE1 | MSR_VEC | MSR_VSX);

#ifdef CONFIG_ALTIVEC
	err |= __get_user(v_regs, &sc->v_regs);
	if (err)
		return err;
	if (v_regs && !access_ok(VERIFY_READ, v_regs, 34 * sizeof(vector128)))
		return -EFAULT;
	 
	if (v_regs != NULL && (msr & MSR_VEC) != 0)
		err |= __copy_from_user(&current->thread.vr_state, v_regs,
					33 * sizeof(vector128));
	else if (current->thread.used_vr)
		memset(&current->thread.vr_state, 0, 33 * sizeof(vector128));
	 
	if (v_regs != NULL)
		err |= __get_user(current->thread.vrsave, (u32 __user *)&v_regs[33]);
	else
		current->thread.vrsave = 0;
	if (cpu_has_feature(CPU_FTR_ALTIVEC))
		mtspr(SPRN_VRSAVE, current->thread.vrsave);
#endif  
	 
	err |= copy_fpr_from_user(current, &sc->fp_regs);
#ifdef CONFIG_VSX
	 
	v_regs += ELF_NVRREG;
	if ((msr & MSR_VSX) != 0)
		err |= copy_vsx_from_user(current, v_regs);
	else
		for (i = 0; i < 32 ; i++)
			current->thread.fp_state.fpr[i][TS_VSRLOWOFFSET] = 0;
#endif
	return err;
}
