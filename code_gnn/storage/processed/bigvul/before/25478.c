static int fpr_get(struct task_struct *target, const struct user_regset *regset,
		   unsigned int pos, unsigned int count,
		   void *kbuf, void __user *ubuf)
{
#ifdef CONFIG_VSX
	double buf[33];
	int i;
#endif
	flush_fp_to_thread(target);

#ifdef CONFIG_VSX
	 
	for (i = 0; i < 32 ; i++)
		buf[i] = target->thread.TS_FPR(i);
	memcpy(&buf[32], &target->thread.fpscr, sizeof(double));
	return user_regset_copyout(&pos, &count, &kbuf, &ubuf, buf, 0, -1);

#else
	BUILD_BUG_ON(offsetof(struct thread_struct, fpscr) !=
		     offsetof(struct thread_struct, TS_FPR(32)));

	return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				   &target->thread.fpr, 0, -1);
#endif
}
