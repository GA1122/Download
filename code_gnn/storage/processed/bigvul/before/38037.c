static int s390_fpregs_set(struct task_struct *target,
			   const struct user_regset *regset, unsigned int pos,
			   unsigned int count, const void *kbuf,
			   const void __user *ubuf)
{
	int rc = 0;

	if (target == current) {
		save_fp_ctl(&target->thread.fp_regs.fpc);
		save_fp_regs(target->thread.fp_regs.fprs);
	}

	 
	if (count > 0 && pos < offsetof(s390_fp_regs, fprs)) {
		u32 ufpc[2] = { target->thread.fp_regs.fpc, 0 };
		rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &ufpc,
					0, offsetof(s390_fp_regs, fprs));
		if (rc)
			return rc;
		if (ufpc[1] != 0 || test_fp_ctl(ufpc[0]))
			return -EINVAL;
		target->thread.fp_regs.fpc = ufpc[0];
	}

	if (rc == 0 && count > 0)
		rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
					target->thread.fp_regs.fprs,
					offsetof(s390_fp_regs, fprs), -1);

	if (rc == 0 && target == current) {
		restore_fp_ctl(&target->thread.fp_regs.fpc);
		restore_fp_regs(target->thread.fp_regs.fprs);
	}

	return rc;
}