static int emulate_instruction(struct pt_regs *regs)
{
	u32 instword;
	u32 rd;

	if (!user_mode(regs) || (regs->msr & MSR_LE))
		return -EINVAL;
	CHECK_FULL_REGS(regs);

	if (get_user(instword, (u32 __user *)(regs->nip)))
		return -EFAULT;

	 
	if ((instword & INST_MFSPR_PVR_MASK) == INST_MFSPR_PVR) {
		rd = (instword >> 21) & 0x1f;
		regs->gpr[rd] = mfspr(SPRN_PVR);
		return 0;
	}

	 
	if ((instword & INST_DCBA_MASK) == INST_DCBA)
		return 0;

	 
	if ((instword & INST_MCRXR_MASK) == INST_MCRXR) {
		int shift = (instword >> 21) & 0x1c;
		unsigned long msk = 0xf0000000UL >> shift;

		regs->ccr = (regs->ccr & ~msk) | ((regs->xer >> shift) & msk);
		regs->xer &= ~0xf0000000UL;
		return 0;
	}

	 
	if ((instword & INST_STRING_GEN_MASK) == INST_STRING)
		return emulate_string_inst(regs, instword);

	 
	if ((instword & INST_POPCNTB_MASK) == INST_POPCNTB) {
		return emulate_popcntb_inst(regs, instword);
	}

	return -EINVAL;
}
