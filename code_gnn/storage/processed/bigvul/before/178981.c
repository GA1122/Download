 int handle_ldf_stq(u32 insn, struct pt_regs *regs)
 {
 	unsigned long addr = compute_effective_address(regs, insn, 0);
 	int freg = ((insn >> 25) & 0x1e) | ((insn >> 20) & 0x20);
 	struct fpustate *f = FPUSTATE;
  	int asi = decode_asi(insn, regs);
  	int flag = (freg < 32) ? FPRS_DL : FPRS_DU;
  
	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, 0, regs, 0);
// 	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, regs, 0);
  
  	save_and_clear_fpu();
  	current_thread_info()->xfsr[0] &= ~0x1c000;
 	if (freg & 3) {
 		current_thread_info()->xfsr[0] |= (6 << 14)  ;
 		do_fpother(regs);
 		return 0;
 	}
 	if (insn & 0x200000) {
 		 
 		u64 first = 0, second = 0;
 		
 		if (current_thread_info()->fpsaved[0] & flag) {
 			first = *(u64 *)&f->regs[freg];
 			second = *(u64 *)&f->regs[freg+2];
 		}
 		if (asi < 0x80) {
 			do_privact(regs);
 			return 1;
 		}
 		switch (asi) {
 		case ASI_P:
 		case ASI_S: break;
 		case ASI_PL:
 		case ASI_SL: 
 			{
 				 
 				u64 tmp = __swab64p(&first);
 				
 				first = __swab64p(&second);
 				second = tmp;
 				break;
 			}
 		default:
 			if (tlb_type == hypervisor)
 				sun4v_data_access_exception(regs, addr, 0);
 			else
 				spitfire_data_access_exception(regs, 0, addr);
 			return 1;
 		}
 		if (put_user (first >> 32, (u32 __user *)addr) ||
 		    __put_user ((u32)first, (u32 __user *)(addr + 4)) ||
 		    __put_user (second >> 32, (u32 __user *)(addr + 8)) ||
 		    __put_user ((u32)second, (u32 __user *)(addr + 12))) {
 			if (tlb_type == hypervisor)
 				sun4v_data_access_exception(regs, addr, 0);
 			else
 				spitfire_data_access_exception(regs, 0, addr);
 		    	return 1;
 		}
 	} else {
 		 
 		u32 data[4] __attribute__ ((aligned(8)));
 		int size, i;
 		int err;
 
 		if (asi < 0x80) {
 			do_privact(regs);
 			return 1;
 		} else if (asi > ASI_SNFL) {
 			if (tlb_type == hypervisor)
 				sun4v_data_access_exception(regs, addr, 0);
 			else
 				spitfire_data_access_exception(regs, 0, addr);
 			return 1;
 		}
 		switch (insn & 0x180000) {
 		case 0x000000: size = 1; break;
 		case 0x100000: size = 4; break;
 		default: size = 2; break;
 		}
 		for (i = 0; i < size; i++)
 			data[i] = 0;
 		
 		err = get_user (data[0], (u32 __user *) addr);
 		if (!err) {
 			for (i = 1; i < size; i++)
 				err |= __get_user (data[i], (u32 __user *)(addr + 4*i));
 		}
 		if (err && !(asi & 0x2  )) {
 			if (tlb_type == hypervisor)
 				sun4v_data_access_exception(regs, addr, 0);
 			else
 				spitfire_data_access_exception(regs, 0, addr);
 			return 1;
 		}
 		if (asi & 0x8)   {
 			u64 tmp;
 
 			switch (size) {
 			case 1: data[0] = le32_to_cpup(data + 0); break;
 			default:*(u64 *)(data + 0) = le64_to_cpup((u64 *)(data + 0));
 				break;
 			case 4: tmp = le64_to_cpup((u64 *)(data + 0));
 				*(u64 *)(data + 0) = le64_to_cpup((u64 *)(data + 2));
 				*(u64 *)(data + 2) = tmp;
 				break;
 			}
 		}
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
 		memcpy(f->regs + freg, data, size * 4);
 		current_thread_info()->fpsaved[0] |= flag;
 	}
 	advance(regs);
 	return 1;
 }