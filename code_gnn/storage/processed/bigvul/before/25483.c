static int gpr_get(struct task_struct *target, const struct user_regset *regset,
		   unsigned int pos, unsigned int count,
		   void *kbuf, void __user *ubuf)
{
	int i, ret;

	if (target->thread.regs == NULL)
		return -EIO;

	if (!FULL_REGS(target->thread.regs)) {
		 
		for (i = 14; i < 32; i++)
			target->thread.regs->gpr[i] = NV_REG_POISON;
	}

	ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				  target->thread.regs,
				  0, offsetof(struct pt_regs, msr));
	if (!ret) {
		unsigned long msr = get_user_msr(target);
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf, &msr,
					  offsetof(struct pt_regs, msr),
					  offsetof(struct pt_regs, msr) +
					  sizeof(msr));
	}

	BUILD_BUG_ON(offsetof(struct pt_regs, orig_gpr3) !=
		     offsetof(struct pt_regs, msr) + sizeof(long));

	if (!ret)
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
					  &target->thread.regs->orig_gpr3,
					  offsetof(struct pt_regs, orig_gpr3),
					  sizeof(struct pt_regs));
	if (!ret)
		ret = user_regset_copyout_zero(&pos, &count, &kbuf, &ubuf,
					       sizeof(struct pt_regs), -1);

	return ret;
}