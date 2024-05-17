SYSCALL_DEFINE2(osf_getitimer, int, which, struct itimerval32 __user *, it)
{
	struct itimerval kit;
	int error;

	error = do_getitimer(which, &kit);
	if (!error && put_it32(it, &kit))
		error = -EFAULT;

	return error;
}
