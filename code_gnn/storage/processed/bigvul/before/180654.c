 static long restore_tm_sigcontexts(struct pt_regs *regs,
 				   struct sigcontext __user *sc,
 				   struct sigcontext __user *tm_sc)
 {
 #ifdef CONFIG_ALTIVEC
 	elf_vrreg_t __user *v_regs, *tm_v_regs;
 #endif
 	unsigned long err = 0;
 	unsigned long msr;
 #ifdef CONFIG_VSX
 	int i;
 #endif
 	 
 	err |= __copy_from_user(regs->gpr, tm_sc->gp_regs, sizeof(regs->gpr));
 	err |= __copy_from_user(&current->thread.ckpt_regs, sc->gp_regs,
 				sizeof(regs->gpr));
 
 	 
 	err |= __get_user(regs->nip, &tm_sc->gp_regs[PT_NIP]);
 	err |= __get_user(current->thread.tm_tfhar, &sc->gp_regs[PT_NIP]);
  
  	 
  	err |= __get_user(msr, &sc->gp_regs[PT_MSR]);
// 	 
// 	if (MSR_TM_RESV(msr))
// 		return -EINVAL;
// 
  	 
  	regs->msr = (regs->msr & ~MSR_TS_MASK) | (msr & MSR_TS_MASK);
  
 	 
 	regs->msr = (regs->msr & ~MSR_LE) | (msr & MSR_LE);
 
 	 
 	err |= __get_user(regs->ctr, &tm_sc->gp_regs[PT_CTR]);
 	err |= __get_user(regs->link, &tm_sc->gp_regs[PT_LNK]);
 	err |= __get_user(regs->xer, &tm_sc->gp_regs[PT_XER]);
 	err |= __get_user(regs->ccr, &tm_sc->gp_regs[PT_CCR]);
 	err |= __get_user(current->thread.ckpt_regs.ctr,
 			  &sc->gp_regs[PT_CTR]);
 	err |= __get_user(current->thread.ckpt_regs.link,
 			  &sc->gp_regs[PT_LNK]);
 	err |= __get_user(current->thread.ckpt_regs.xer,
 			  &sc->gp_regs[PT_XER]);
 	err |= __get_user(current->thread.ckpt_regs.ccr,
 			  &sc->gp_regs[PT_CCR]);
 
 	 
 	err |= __get_user(regs->trap, &sc->gp_regs[PT_TRAP]);
 	err |= __get_user(regs->dar, &sc->gp_regs[PT_DAR]);
 	err |= __get_user(regs->dsisr, &sc->gp_regs[PT_DSISR]);
 	err |= __get_user(regs->result, &sc->gp_regs[PT_RESULT]);
 
 	 
 	discard_lazy_cpu_state();
 
 	 
 	regs->msr &= ~(MSR_FP | MSR_FE0 | MSR_FE1 | MSR_VEC | MSR_VSX);
 
 #ifdef CONFIG_ALTIVEC
 	err |= __get_user(v_regs, &sc->v_regs);
 	err |= __get_user(tm_v_regs, &tm_sc->v_regs);
 	if (err)
 		return err;
 	if (v_regs && !access_ok(VERIFY_READ, v_regs, 34 * sizeof(vector128)))
 		return -EFAULT;
 	if (tm_v_regs && !access_ok(VERIFY_READ,
 				    tm_v_regs, 34 * sizeof(vector128)))
 		return -EFAULT;
 	 
 	if (v_regs != NULL && tm_v_regs != NULL && (msr & MSR_VEC) != 0) {
 		err |= __copy_from_user(&current->thread.vr_state, v_regs,
 					33 * sizeof(vector128));
 		err |= __copy_from_user(&current->thread.transact_vr, tm_v_regs,
 					33 * sizeof(vector128));
 	}
 	else if (current->thread.used_vr) {
 		memset(&current->thread.vr_state, 0, 33 * sizeof(vector128));
 		memset(&current->thread.transact_vr, 0, 33 * sizeof(vector128));
 	}
 	 
 	if (v_regs != NULL && tm_v_regs != NULL) {
 		err |= __get_user(current->thread.vrsave,
 				  (u32 __user *)&v_regs[33]);
 		err |= __get_user(current->thread.transact_vrsave,
 				  (u32 __user *)&tm_v_regs[33]);
 	}
 	else {
 		current->thread.vrsave = 0;
 		current->thread.transact_vrsave = 0;
 	}
 	if (cpu_has_feature(CPU_FTR_ALTIVEC))
 		mtspr(SPRN_VRSAVE, current->thread.vrsave);
 #endif  
 	 
 	err |= copy_fpr_from_user(current, &sc->fp_regs);
 	err |= copy_transact_fpr_from_user(current, &tm_sc->fp_regs);
 #ifdef CONFIG_VSX
 	 
 	if (v_regs && ((msr & MSR_VSX) != 0)) {
 		v_regs += ELF_NVRREG;
 		tm_v_regs += ELF_NVRREG;
 		err |= copy_vsx_from_user(current, v_regs);
 		err |= copy_transact_vsx_from_user(current, tm_v_regs);
 	} else {
 		for (i = 0; i < 32 ; i++) {
 			current->thread.fp_state.fpr[i][TS_VSRLOWOFFSET] = 0;
 			current->thread.transact_fp.fpr[i][TS_VSRLOWOFFSET] = 0;
 		}
 	}
 #endif
 	tm_enable();
 	 
 	current->thread.tm_texasr |= TEXASR_FS;
 	 
 	tm_recheckpoint(&current->thread, msr);
 
 	 
 	if (msr & MSR_FP) {
 		do_load_up_transact_fpu(&current->thread);
 		regs->msr |= (MSR_FP | current->thread.fpexc_mode);
 	}
 #ifdef CONFIG_ALTIVEC
 	if (msr & MSR_VEC) {
 		do_load_up_transact_altivec(&current->thread);
 		regs->msr |= MSR_VEC;
 	}
 #endif
 
 	return err;
 }