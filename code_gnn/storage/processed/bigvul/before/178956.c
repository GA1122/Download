 asmlinkage void do_ade(struct pt_regs *regs)
 {
 	unsigned int __user *pc;
  	mm_segment_t seg;
  
  	perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS,
			1, 0, regs, regs->cp0_badvaddr);
// 			1, regs, regs->cp0_badvaddr);
  	 
 	if ((regs->cp0_badvaddr == regs->cp0_epc) || (regs->cp0_epc & 0x1))
 		goto sigbus;
 
 	pc = (unsigned int __user *) exception_epc(regs);
 	if (user_mode(regs) && !test_thread_flag(TIF_FIXADE))
 		goto sigbus;
 	if (unaligned_action == UNALIGNED_ACTION_SIGNAL)
 		goto sigbus;
 	else if (unaligned_action == UNALIGNED_ACTION_SHOW)
 		show_registers(regs);
 
 	 
 	seg = get_fs();
 	if (!user_mode(regs))
 		set_fs(KERNEL_DS);
 	emulate_load_store_insn(regs, (void __user *)regs->cp0_badvaddr, pc);
 	set_fs(seg);
 
 	return;
 
 sigbus:
 	die_if_kernel("Kernel unaligned instruction access", regs);
 	force_sig(SIGBUS, current);
 
 	 
 }