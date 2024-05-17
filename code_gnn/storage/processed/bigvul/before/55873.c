void no_tty(void)
{
	 
	struct task_struct *tsk = current;
	disassociate_ctty(0);
	proc_clear_tty(tsk);
}
