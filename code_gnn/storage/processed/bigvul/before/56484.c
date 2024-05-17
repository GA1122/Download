static int save_tm_user_regs(struct pt_regs *regs,
			     struct mcontext __user *frame,
			     struct mcontext __user *tm_frame, int sigret)
{
	unsigned long msr = regs->msr;

	 
	regs->msr &= ~MSR_TS_MASK;

	 
	flush_fp_to_thread(current);

	 
	if (save_general_regs(&current->thread.ckpt_regs, frame)
	    || save_general_regs(regs, tm_frame))
		return 1;

	 
	if (__put_user((msr >> 32), &tm_frame->mc_gregs[PT_MSR]))
		return 1;

#ifdef CONFIG_ALTIVEC
	 
	if (current->thread.used_vr) {
		flush_altivec_to_thread(current);
		if (__copy_to_user(&frame->mc_vregs, &current->thread.vr_state,
				   ELF_NVRREG * sizeof(vector128)))
			return 1;
		if (msr & MSR_VEC) {
			if (__copy_to_user(&tm_frame->mc_vregs,
					   &current->thread.transact_vr,
					   ELF_NVRREG * sizeof(vector128)))
				return 1;
		} else {
			if (__copy_to_user(&tm_frame->mc_vregs,
					   &current->thread.vr_state,
					   ELF_NVRREG * sizeof(vector128)))
				return 1;
		}

		 
		msr |= MSR_VEC;
	}

	 
	if (cpu_has_feature(CPU_FTR_ALTIVEC))
		current->thread.vrsave = mfspr(SPRN_VRSAVE);
	if (__put_user(current->thread.vrsave,
		       (u32 __user *)&frame->mc_vregs[32]))
		return 1;
	if (msr & MSR_VEC) {
		if (__put_user(current->thread.transact_vrsave,
			       (u32 __user *)&tm_frame->mc_vregs[32]))
			return 1;
	} else {
		if (__put_user(current->thread.vrsave,
			       (u32 __user *)&tm_frame->mc_vregs[32]))
			return 1;
	}
#endif  

	if (copy_fpr_to_user(&frame->mc_fregs, current))
		return 1;
	if (msr & MSR_FP) {
		if (copy_transact_fpr_to_user(&tm_frame->mc_fregs, current))
			return 1;
	} else {
		if (copy_fpr_to_user(&tm_frame->mc_fregs, current))
			return 1;
	}

#ifdef CONFIG_VSX
	 
	if (current->thread.used_vsr) {
		__giveup_vsx(current);
		if (copy_vsx_to_user(&frame->mc_vsregs, current))
			return 1;
		if (msr & MSR_VSX) {
			if (copy_transact_vsx_to_user(&tm_frame->mc_vsregs,
						      current))
				return 1;
		} else {
			if (copy_vsx_to_user(&tm_frame->mc_vsregs, current))
				return 1;
		}

		msr |= MSR_VSX;
	}
#endif  
#ifdef CONFIG_SPE
	 
	if (current->thread.used_spe) {
		flush_spe_to_thread(current);
		if (__copy_to_user(&frame->mc_vregs, current->thread.evr,
				   ELF_NEVRREG * sizeof(u32)))
			return 1;
		 
		msr |= MSR_SPE;
	}

	 
	if (__put_user(current->thread.spefscr, (u32 __user *)&frame->mc_vregs + ELF_NEVRREG))
		return 1;
#endif  

	if (__put_user(msr, &frame->mc_gregs[PT_MSR]))
		return 1;
	if (sigret) {
		 
		if (__put_user(0x38000000UL + sigret, &frame->tramp[0])
		    || __put_user(0x44000002UL, &frame->tramp[1]))
			return 1;
		flush_icache_range((unsigned long) &frame->tramp[0],
				   (unsigned long) &frame->tramp[2]);
	}

	return 0;
}
