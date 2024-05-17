static inline int save_general_regs(struct pt_regs *regs,
		struct mcontext __user *frame)
{
	elf_greg_t64 *gregs = (elf_greg_t64 *)regs;
	int i;

	WARN_ON(!FULL_REGS(regs));

	for (i = 0; i <= PT_RESULT; i ++) {
		if (i == 14 && !FULL_REGS(regs))
			i = 32;
		if (__put_user((unsigned int)gregs[i], &frame->mc_gregs[i]))
			return -EFAULT;
	}
	return 0;
}
