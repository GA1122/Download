send_sig_info(int sig, struct siginfo *info, struct task_struct *p)
{
	 
	if (!valid_signal(sig))
		return -EINVAL;

	return do_send_sig_info(sig, info, p, false);
}
