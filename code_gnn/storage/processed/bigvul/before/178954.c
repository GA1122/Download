 static int simulate_rdhwr(struct pt_regs *regs, unsigned int opcode)
 {
 	struct thread_info *ti = task_thread_info(current);
 
 	if ((opcode & OPCODE) == SPEC3 && (opcode & FUNC) == RDHWR) {
  		int rd = (opcode & RD) >> 11;
  		int rt = (opcode & RT) >> 16;
  		perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS,
				1, 0, regs, 0);
// 				1, regs, 0);
  		switch (rd) {
  		case 0:		 
  			regs->regs[rt] = smp_processor_id();
 			return 0;
 		case 1:		 
 			regs->regs[rt] = min(current_cpu_data.dcache.linesz,
 					     current_cpu_data.icache.linesz);
 			return 0;
 		case 2:		 
 			regs->regs[rt] = read_c0_count();
 			return 0;
 		case 3:		 
 			switch (current_cpu_data.cputype) {
 			case CPU_20KC:
 			case CPU_25KF:
 				regs->regs[rt] = 1;
 				break;
 			default:
 				regs->regs[rt] = 2;
 			}
 			return 0;
 		case 29:
 			regs->regs[rt] = ti->tp_value;
 			return 0;
 		default:
 			return -1;
 		}
 	}
 
 	 
 	return -1;
 }