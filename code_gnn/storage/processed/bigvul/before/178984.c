 asmlinkage void kernel_unaligned_trap(struct pt_regs *regs, unsigned int insn)
 {
 	enum direction dir = decode_direction(insn);
 	int size = decode_access_size(regs, insn);
 	int orig_asi, asi;
 
 	current_thread_info()->kern_una_regs = regs;
 	current_thread_info()->kern_una_insn = insn;
 
 	orig_asi = asi = decode_asi(insn, regs);
 
 	 
 	if (asi == ASI_AIUS) {
 		kernel_mna_trap_fault(0);
 		return;
 	}
 
 	log_unaligned(regs);
 
 	if (!ok_for_kernel(insn) || dir == both) {
 		printk("Unsupported unaligned load/store trap for kernel "
 		       "at <%016lx>.\n", regs->tpc);
 		unaligned_panic("Kernel does fpu/atomic "
 				"unaligned load/store.", regs);
 
 		kernel_mna_trap_fault(0);
 	} else {
 		unsigned long addr, *reg_addr;
 		int err;
  
  		addr = compute_effective_address(regs, insn,
  						 ((insn >> 25) & 0x1f));
		perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, 0, regs, addr);
// 		perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, regs, addr);
  		switch (asi) {
  		case ASI_NL:
  		case ASI_AIUPL:
 		case ASI_AIUSL:
 		case ASI_PL:
 		case ASI_SL:
 		case ASI_PNFL:
 		case ASI_SNFL:
 			asi &= ~0x08;
 			break;
 		}
 		switch (dir) {
 		case load:
 			reg_addr = fetch_reg_addr(((insn>>25)&0x1f), regs);
 			err = do_int_load(reg_addr, size,
 					  (unsigned long *) addr,
 					  decode_signedness(insn), asi);
 			if (likely(!err) && unlikely(asi != orig_asi)) {
 				unsigned long val_in = *reg_addr;
 				switch (size) {
 				case 2:
 					val_in = swab16(val_in);
 					break;
 				case 4:
 					val_in = swab32(val_in);
 					break;
 				case 8:
 					val_in = swab64(val_in);
 					break;
 				case 16:
 				default:
 					BUG();
 					break;
 				}
 				*reg_addr = val_in;
 			}
 			break;
 
 		case store:
 			err = do_int_store(((insn>>25)&0x1f), size,
 					   (unsigned long *) addr, regs,
 					   asi, orig_asi);
 			break;
 
 		default:
 			panic("Impossible kernel unaligned trap.");
 			 
 		}
 		if (unlikely(err))
 			kernel_mna_trap_fault(1);
 		else
 			advance(regs);
 	}
 }