 int handle_popc(u32 insn, struct pt_regs *regs)
 {
 	u64 value;
  	int ret, i, rd = ((insn >> 25) & 0x1f);
  	int from_kernel = (regs->tstate & TSTATE_PRIV) != 0;
  	                        
	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, 0, regs, 0);
// 	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, regs, 0);
  	if (insn & 0x2000) {
  		maybe_flush_windows(0, 0, rd, from_kernel);
  		value = sign_extend_imm13(insn);
 	} else {
 		maybe_flush_windows(0, insn & 0x1f, rd, from_kernel);
 		value = fetch_reg(insn & 0x1f, regs);
 	}
 	for (ret = 0, i = 0; i < 16; i++) {
 		ret += popc_helper[value & 0xf];
 		value >>= 4;
 	}
 	if (rd < 16) {
 		if (rd)
 			regs->u_regs[rd] = ret;
 	} else {
 		if (test_thread_flag(TIF_32BIT)) {
 			struct reg_window32 __user *win32;
 			win32 = (struct reg_window32 __user *)((unsigned long)((u32)regs->u_regs[UREG_FP]));
 			put_user(ret, &win32->locals[rd - 16]);
 		} else {
 			struct reg_window __user *win;
 			win = (struct reg_window __user *)(regs->u_regs[UREG_FP] + STACK_BIAS);
 			put_user(ret, &win->locals[rd - 16]);
 		}
 	}
 	advance(regs);
 	return 1;
 }