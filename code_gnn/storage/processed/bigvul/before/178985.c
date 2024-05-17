 int vis_emul(struct pt_regs *regs, unsigned int insn)
 {
 	unsigned long pc = regs->tpc;
 	unsigned int opf;
  
  	BUG_ON(regs->tstate & TSTATE_PRIV);
  
	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, 0, regs, 0);
// 	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS, 1, regs, 0);
  
  	if (test_thread_flag(TIF_32BIT))
  		pc = (u32)pc;
 
 	if (get_user(insn, (u32 __user *) pc))
 		return -EFAULT;
 
 	save_and_clear_fpu();
 
 	opf = (insn & VIS_OPF_MASK) >> VIS_OPF_SHIFT;
 	switch (opf) {
 	default:
 		return -EINVAL;
 
 	 
 	case FPACK16_OPF:
 	case FPACK32_OPF:
 	case FPACKFIX_OPF:
 	case FEXPAND_OPF:
 	case FPMERGE_OPF:
 		pformat(regs, insn, opf);
 		break;
 
 	 
 	case FMUL8x16_OPF:
 	case FMUL8x16AU_OPF:
 	case FMUL8x16AL_OPF:
 	case FMUL8SUx16_OPF:
 	case FMUL8ULx16_OPF:
 	case FMULD8SUx16_OPF:
 	case FMULD8ULx16_OPF:
 		pmul(regs, insn, opf);
 		break;
 
 	 
 	case FCMPGT16_OPF:
 	case FCMPGT32_OPF:
 	case FCMPLE16_OPF:
 	case FCMPLE32_OPF:
 	case FCMPNE16_OPF:
 	case FCMPNE32_OPF:
 	case FCMPEQ16_OPF:
 	case FCMPEQ32_OPF:
 		pcmp(regs, insn, opf);
 		break;
 
 	 
 	case EDGE8_OPF:
 	case EDGE8N_OPF:
 	case EDGE8L_OPF:
 	case EDGE8LN_OPF:
 	case EDGE16_OPF:
 	case EDGE16N_OPF:
 	case EDGE16L_OPF:
 	case EDGE16LN_OPF:
 	case EDGE32_OPF:
 	case EDGE32N_OPF:
 	case EDGE32L_OPF:
 	case EDGE32LN_OPF:
 		edge(regs, insn, opf);
 		break;
 
 	 
 	case PDIST_OPF:
 		pdist(regs, insn);
 		break;
 
 	 
 	case ARRAY8_OPF:
 	case ARRAY16_OPF:
 	case ARRAY32_OPF:
 		array(regs, insn, opf);
 		break;
 
 	 
 	case BMASK_OPF:
 		bmask(regs, insn);
 		break;
 
 	case BSHUFFLE_OPF:
 		bshuffle(regs, insn);
 		break;
 	}
 
 	regs->tpc = regs->tnpc;
 	regs->tnpc += 4;
 	return 0;
 }