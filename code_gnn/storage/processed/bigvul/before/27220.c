SYSCALL_DEFINE2(osf_settimeofday, struct timeval32 __user *, tv,
		struct timezone __user *, tz)
{
	struct timespec kts;
	struct timezone ktz;

 	if (tv) {
		if (get_tv32((struct timeval *)&kts, tv))
			return -EFAULT;
	}
	if (tz) {
		if (copy_from_user(&ktz, tz, sizeof(*tz)))
			return -EFAULT;
	}

	kts.tv_nsec *= 1000;

	return do_sys_settimeofday(tv ? &kts : NULL, tz ? &ktz : NULL);
}
