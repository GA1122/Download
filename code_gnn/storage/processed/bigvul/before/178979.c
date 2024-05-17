 void handle_ld_nf(u32 insn, struct pt_regs *regs)
 {
 	int rd = ((insn >> 25) & 0x1f);
  	int from_kernel = (regs->tstate & TSTATE_PRIV) != 0;
  	unsigned long *reg;
  	                        
	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, 0, regs, 0);
// 	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, regs, 0);
  
  	maybe_flush_windows(0, 0, rd, from_kernel);
  	reg = fetch_reg_addr(rd, regs);
 	if (from_kernel || rd < 16) {
 		reg[0] = 0;
 		if ((insn & 0x780000) == 0x180000)
 			reg[1] = 0;
 	} else if (test_thread_flag(TIF_32BIT)) {
 		put_user(0, (int __user *) reg);
 		if ((insn & 0x780000) == 0x180000)
 			put_user(0, ((int __user *) reg) + 1);
 	} else {
 		put_user(0, (unsigned long __user *) reg);
 		if ((insn & 0x780000) == 0x180000)
 			put_user(0, (unsigned long __user *) reg + 1);
 	}
 	advance(regs);
 }