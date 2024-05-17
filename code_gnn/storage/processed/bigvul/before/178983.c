 void handle_stdfmna(struct pt_regs *regs, unsigned long sfar, unsigned long sfsr)
 {
 	unsigned long pc = regs->tpc;
 	unsigned long tstate = regs->tstate;
 	u32 insn;
 	u64 value;
 	u8 freg;
 	int flag;
 	struct fpustate *f = FPUSTATE;
  
  	if (tstate & TSTATE_PRIV)
  		die_if_kernel("stdfmna from kernel", regs);
	perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, 0, regs, sfar);
// 	perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, regs, sfar);
  	if (test_thread_flag(TIF_32BIT))
  		pc = (u32)pc;
  	if (get_user(insn, (u32 __user *) pc) != -EFAULT) {
 		int asi = decode_asi(insn, regs);
 		freg = ((insn >> 25) & 0x1e) | ((insn >> 20) & 0x20);
 		value = 0;
 		flag = (freg < 32) ? FPRS_DL : FPRS_DU;
 		if ((asi > ASI_SNFL) ||
 		    (asi < ASI_P))
 			goto daex;
 		save_and_clear_fpu();
 		if (current_thread_info()->fpsaved[0] & flag)
 			value = *(u64 *)&f->regs[freg];
 		switch (asi) {
 		case ASI_P:
 		case ASI_S: break;
 		case ASI_PL:
 		case ASI_SL: 
 			value = __swab64p(&value); break;
 		default: goto daex;
 		}
 		if (put_user (value >> 32, (u32 __user *) sfar) ||
 		    __put_user ((u32)value, (u32 __user *)(sfar + 4)))
 			goto daex;
 	} else {
 daex:
 		if (tlb_type == hypervisor)
 			sun4v_data_access_exception(regs, sfar, sfsr);
 		else
 			spitfire_data_access_exception(regs, sfsr, sfar);
 		return;
 	}
 	advance(regs);
 }