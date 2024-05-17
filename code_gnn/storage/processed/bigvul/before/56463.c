unsigned long copy_transact_fpr_from_user(struct task_struct *task,
					  void __user *from)
{
	u64 buf[ELF_NFPREG];
	int i;

	if (__copy_from_user(buf, from, ELF_NFPREG * sizeof(double)))
		return 1;
	for (i = 0; i < (ELF_NFPREG - 1) ; i++)
		task->thread.TS_TRANS_FPR(i) = buf[i];
	task->thread.transact_fp.fpscr = buf[i];

	return 0;
}
