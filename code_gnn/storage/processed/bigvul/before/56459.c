unsigned long copy_fpr_to_user(void __user *to,
			       struct task_struct *task)
{
	u64 buf[ELF_NFPREG];
	int i;

	 
	for (i = 0; i < (ELF_NFPREG - 1) ; i++)
		buf[i] = task->thread.TS_FPR(i);
	buf[i] = task->thread.fp_state.fpscr;
	return __copy_to_user(to, buf, ELF_NFPREG * sizeof(double));
}
