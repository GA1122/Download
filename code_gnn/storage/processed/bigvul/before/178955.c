 static int simulate_sync(struct pt_regs *regs, unsigned int opcode)
  {
  	if ((opcode & OPCODE) == SPEC0 && (opcode & FUNC) == SYNC) {
  		perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS,
				1, 0, regs, 0);
// 				1, regs, 0);
  		return 0;
  	}
  
 	return -1;			 
 }