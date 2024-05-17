 void handle_lddfmna(struct pt_regs *regs, unsigned long sfar, unsigned long sfsr)
 {
 	unsigned long pc = regs->tpc;
 	unsigned long tstate = regs->tstate;
 	u32 insn;
 	u64 value;
 	u8 freg;
 	int flag;
 	struct fpustate *f = FPUSTATE;
  
  	if (tstate & TSTATE_PRIV)
  		die_if_kernel("lddfmna from kernel", regs);
	perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, 0, regs, sfar);
// 	perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, regs, sfar);
  	if (test_thread_flag(TIF_32BIT))
  		pc = (u32)pc;
  	if (get_user(insn, (u32 __user *) pc) != -EFAULT) {
 		int asi = decode_asi(insn, regs);
 		u32 first, second;
 		int err;
 
 		if ((asi > ASI_SNFL) ||
 		    (asi < ASI_P))
 			goto daex;
 		first = second = 0;
 		err = get_user(first, (u32 __user *)sfar);
 		if (!err)
 			err = get_user(second, (u32 __user *)(sfar + 4));
 		if (err) {
 			if (!(asi & 0x2))
 				goto daex;
 			first = second = 0;
 		}
 		save_and_clear_fpu();
 		freg = ((insn >> 25) & 0x1e) | ((insn >> 20) & 0x20);
 		value = (((u64)first) << 32) | second;
 		if (asi & 0x8)  
 			value = __swab64p(&value);
 		flag = (freg < 32) ? FPRS_DL : FPRS_DU;
 		if (!(current_thread_info()->fpsaved[0] & FPRS_FEF)) {
 			current_thread_info()->fpsaved[0] = FPRS_FEF;
 			current_thread_info()->gsr[0] = 0;
 		}
 		if (!(current_thread_info()->fpsaved[0] & flag)) {
 			if (freg < 32)
 				memset(f->regs, 0, 32*sizeof(u32));
 			else
 				memset(f->regs+32, 0, 32*sizeof(u32));
 		}
 		*(u64 *)(f->regs + freg) = value;
 		current_thread_info()->fpsaved[0] |= flag;
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