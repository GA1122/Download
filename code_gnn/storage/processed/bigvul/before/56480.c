static inline int restore_general_regs(struct pt_regs *regs,
		struct mcontext __user *sr)
{
	 
	if (__copy_from_user(regs, &sr->mc_gregs,
				PT_MSR * sizeof(elf_greg_t)))
		return -EFAULT;
	 
	if (__copy_from_user(&regs->orig_gpr3, &sr->mc_gregs[PT_ORIG_R3],
				GP_REGS_SIZE - PT_ORIG_R3 * sizeof(elf_greg_t)))
		return -EFAULT;
	return 0;
}
