unsigned long copy_vsx_to_user(void __user *to,
			       struct task_struct *task)
{
	u64 buf[ELF_NVSRHALFREG];
	int i;

	 
	for (i = 0; i < ELF_NVSRHALFREG; i++)
		buf[i] = task->thread.fp_state.fpr[i][TS_VSRLOWOFFSET];
	return __copy_to_user(to, buf, ELF_NVSRHALFREG * sizeof(double));
}