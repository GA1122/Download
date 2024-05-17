static inline int ok_for_user(struct pt_regs *regs, unsigned int insn,
			      enum direction dir)
{
	unsigned int reg;
	int check = (dir == load) ? VERIFY_READ : VERIFY_WRITE;
	int size = ((insn >> 19) & 3) == 3 ? 8 : 4;

	if ((regs->pc | regs->npc) & 3)
		return 0;

	 
#define WINREG_ADDR(regnum) \
	((void __user *)(((unsigned long *)regs->u_regs[UREG_FP])+(regnum)))

	reg = (insn >> 25) & 0x1f;
	if (reg >= 16) {
		if (!access_ok(check, WINREG_ADDR(reg - 16), size))
			return -EFAULT;
	}
	reg = (insn >> 14) & 0x1f;
	if (reg >= 16) {
		if (!access_ok(check, WINREG_ADDR(reg - 16), size))
			return -EFAULT;
	}
	if (!(insn & 0x2000)) {
		reg = (insn & 0x1f);
		if (reg >= 16) {
			if (!access_ok(check, WINREG_ADDR(reg - 16), size))
				return -EFAULT;
		}
	}
#undef WINREG_ADDR
	return 0;
}