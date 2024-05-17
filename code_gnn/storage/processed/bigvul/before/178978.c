 asmlinkage void user_unaligned_trap(struct pt_regs *regs, unsigned int insn)
 {
 	enum direction dir;
 
 	if(!(current->thread.flags & SPARC_FLAG_UNALIGNED) ||
 	   (((insn >> 30) & 3) != 3))
 		goto kill_user;
 	dir = decode_direction(insn);
 	if(!ok_for_user(regs, insn, dir)) {
 		goto kill_user;
 	} else {
 		int err, size = decode_access_size(insn);
 		unsigned long addr;
 
 		if(floating_point_load_or_store_p(insn)) {
 			printk("User FPU load/store unaligned unsupported.\n");
 			goto kill_user;
  		}
  
  		addr = compute_effective_address(regs, insn);
		perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, 0, regs, addr);
// 		perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS, 1, regs, addr);
  		switch(dir) {
  		case load:
  			err = do_int_load(fetch_reg_addr(((insn>>25)&0x1f),
 							 regs),
 					  size, (unsigned long *) addr,
 					  decode_signedness(insn));
 			break;
 
 		case store:
 			err = do_int_store(((insn>>25)&0x1f), size,
 					   (unsigned long *) addr, regs);
 			break;
 
 		case both:
 			 
 			printk("Unaligned SWAP unsupported.\n");
 			err = -EFAULT;
 			break;
 
 		default:
 			unaligned_panic("Impossible user unaligned trap.");
 			goto out;
 		}
 		if (err)
 			goto kill_user;
 		else
 			advance(regs);
 		goto out;
 	}
 
 kill_user:
 	user_mna_trap_fault(regs, insn);
 out:
 	;
 }