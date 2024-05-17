static int vsr_set(struct task_struct *target, const struct user_regset *regset,
		   unsigned int pos, unsigned int count,
		   const void *kbuf, const void __user *ubuf)
{
	double buf[32];
	int ret,i;

	flush_vsx_to_thread(target);

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				 buf, 0, 32 * sizeof(double));
	for (i = 0; i < 32 ; i++)
		target->thread.fpr[i][TS_VSRLOWOFFSET] = buf[i];


	return ret;
}
