static inline int restore_general_regs(struct pt_regs *regs,
		struct mcontext __user *sr)
{
	elf_greg_t64 *gregs = (elf_greg_t64 *)regs;
	int i;

	for (i = 0; i <= PT_RESULT; i++) {
		if ((i == PT_MSR) || (i == PT_SOFTE))
			continue;
		if (__get_user(gregs[i], &sr->mc_gregs[i]))
			return -EFAULT;
	}
	return 0;
}
