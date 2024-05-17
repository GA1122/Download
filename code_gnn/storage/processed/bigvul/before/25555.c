static int emulate_branch(unsigned short inst, struct pt_regs *regs)
{
	 
	if (((inst & 0xf000) == 0xb000)  ||	 
	    ((inst & 0xf0ff) == 0x0003)  ||	 
	    ((inst & 0xf0ff) == 0x400b))	 
		regs->pr = regs->pc + 4;

	if ((inst & 0xfd00) == 0x8d00) {	 
		regs->pc += SH_PC_8BIT_OFFSET(inst);
		return 0;
	}

	if ((inst & 0xe000) == 0xa000) {	 
		regs->pc += SH_PC_12BIT_OFFSET(inst);
		return 0;
	}

	if ((inst & 0xf0df) == 0x0003) {	 
		regs->pc += regs->regs[(inst & 0x0f00) >> 8] + 4;
		return 0;
	}

	if ((inst & 0xf0df) == 0x400b) {	 
		regs->pc = regs->regs[(inst & 0x0f00) >> 8];
		return 0;
	}

	if ((inst & 0xffff) == 0x000b) {	 
		regs->pc = regs->pr;
		return 0;
	}

	return 1;
}
