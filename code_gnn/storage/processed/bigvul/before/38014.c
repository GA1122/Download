static u32 __peek_user_compat(struct task_struct *child, addr_t addr)
{
	struct compat_user *dummy32 = NULL;
	addr_t offset;
	__u32 tmp;

	if (addr < (addr_t) &dummy32->regs.acrs) {
		struct pt_regs *regs = task_pt_regs(child);
		 
		if (addr == (addr_t) &dummy32->regs.psw.mask) {
			 
			tmp = (__u32)(regs->psw.mask >> 32);
			tmp &= PSW32_MASK_USER | PSW32_MASK_RI;
			tmp |= PSW32_USER_BITS;
		} else if (addr == (addr_t) &dummy32->regs.psw.addr) {
			 
			tmp = (__u32) regs->psw.addr |
				(__u32)(regs->psw.mask & PSW_MASK_BA);
		} else {
			 
			tmp = *(__u32 *)((addr_t) &regs->psw + addr*2 + 4);
		}
	} else if (addr < (addr_t) (&dummy32->regs.orig_gpr2)) {
		 
		offset = addr - (addr_t) &dummy32->regs.acrs;
		tmp = *(__u32*)((addr_t) &child->thread.acrs + offset);

	} else if (addr == (addr_t) (&dummy32->regs.orig_gpr2)) {
		 
		tmp = *(__u32*)((addr_t) &task_pt_regs(child)->orig_gpr2 + 4);

	} else if (addr < (addr_t) &dummy32->regs.fp_regs) {
		 
		tmp = 0;

	} else if (addr < (addr_t) (&dummy32->regs.fp_regs + 1)) {
		 
	        offset = addr - (addr_t) &dummy32->regs.fp_regs;
		tmp = *(__u32 *)((addr_t) &child->thread.fp_regs + offset);

	} else if (addr < (addr_t) (&dummy32->regs.per_info + 1)) {
		 
		addr -= (addr_t) &dummy32->regs.per_info;
		tmp = __peek_user_per_compat(child, addr);

	} else
		tmp = 0;

	return tmp;
}