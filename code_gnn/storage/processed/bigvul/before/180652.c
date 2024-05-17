 static void tm_reclaim_thread(struct thread_struct *thr,
 			      struct thread_info *ti, uint8_t cause)
 {
 	unsigned long msr_diff = 0;
 
 	 
 	if (test_ti_thread_flag(ti, TIF_RESTORE_TM)) {
 		msr_diff = thr->ckpt_regs.msr & ~thr->regs->msr;
 		if (msr_diff & MSR_FP)
 			memcpy(&thr->transact_fp, &thr->fp_state,
 			       sizeof(struct thread_fp_state));
 		if (msr_diff & MSR_VEC)
 			memcpy(&thr->transact_vr, &thr->vr_state,
 			       sizeof(struct thread_vr_state));
 		clear_ti_thread_flag(ti, TIF_RESTORE_TM);
  		msr_diff &= MSR_FP | MSR_VEC | MSR_VSX | MSR_FE0 | MSR_FE1;
  	}
  
// 	 
// 	if (!MSR_TM_SUSPENDED(mfmsr()))
// 		return;
// 
  	tm_reclaim(thr, thr->regs->msr, cause);
  
  	 
 	thr->regs->msr |= msr_diff;
 }