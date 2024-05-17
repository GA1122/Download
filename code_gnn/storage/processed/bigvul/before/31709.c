SYSCALL_DEFINE2(tkill, pid_t, pid, int, sig)
{
	 
	if (pid <= 0)
		return -EINVAL;

	return do_tkill(0, pid, sig);
}
