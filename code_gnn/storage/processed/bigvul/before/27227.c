SYSCALL_DEFINE3(osf_setitimer, int, which, struct itimerval32 __user *, in,
		struct itimerval32 __user *, out)
{
	struct itimerval kin, kout;
	int error;

	if (in) {
		if (get_it32(&kin, in))
			return -EFAULT;
	} else
		memset(&kin, 0, sizeof(kin));

	error = do_setitimer(which, &kin, out ? &kout : NULL);
	if (error || !out)
		return error;

	if (put_it32(out, &kout))
		return -EFAULT;

	return 0;

}
