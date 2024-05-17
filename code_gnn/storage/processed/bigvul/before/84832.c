static int tm_spr_set(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      const void *kbuf, const void __user *ubuf)
{
	int ret;

	 
	BUILD_BUG_ON(TSO(tm_tfhar) + sizeof(u64) != TSO(tm_texasr));
	BUILD_BUG_ON(TSO(tm_texasr) + sizeof(u64) != TSO(tm_tfiar));
	BUILD_BUG_ON(TSO(tm_tfiar) + sizeof(u64) != TSO(ckpt_regs));

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	 
	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);

	 
	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				&target->thread.tm_tfhar, 0, sizeof(u64));

	 
	if (!ret)
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				&target->thread.tm_texasr, sizeof(u64),
				2 * sizeof(u64));

	 
	if (!ret)
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				&target->thread.tm_tfiar,
				 2 * sizeof(u64), 3 * sizeof(u64));
	return ret;
}
