static int genregs_get(struct task_struct *target,
		       const struct user_regset *regset,
		       unsigned int pos, unsigned int count,
		       void *kbuf, void __user *ubuf)
{
	const struct pt_regs *regs = task_pt_regs(target);
	int ret;

	ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				  regs->regs,
				  0, 16 * sizeof(unsigned long));
	if (!ret)
		 
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
					  &regs->pc,
					  offsetof(struct pt_regs, pc),
					  sizeof(struct pt_regs));
	if (!ret)
		ret = user_regset_copyout_zero(&pos, &count, &kbuf, &ubuf,
					       sizeof(struct pt_regs), -1);

	return ret;
}