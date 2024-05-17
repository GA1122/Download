static int job_control(struct tty_struct *tty, struct file *file)
{
	 
	 
	 
	if (file->f_op->write == redirected_tty_write)
		return 0;

	return __tty_check_change(tty, SIGTTIN);
}
