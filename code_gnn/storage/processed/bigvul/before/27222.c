SYSCALL_DEFINE2(osf_utimes, const char __user *, filename,
		struct timeval32 __user *, tvs)
{
	struct timespec tv[2];

	if (tvs) {
		struct timeval ktvs[2];
		if (get_tv32(&ktvs[0], &tvs[0]) ||
		    get_tv32(&ktvs[1], &tvs[1]))
			return -EFAULT;

		if (ktvs[0].tv_usec < 0 || ktvs[0].tv_usec >= 1000000 ||
		    ktvs[1].tv_usec < 0 || ktvs[1].tv_usec >= 1000000)
			return -EINVAL;

		tv[0].tv_sec = ktvs[0].tv_sec;
		tv[0].tv_nsec = 1000 * ktvs[0].tv_usec;
		tv[1].tv_sec = ktvs[1].tv_sec;
		tv[1].tv_nsec = 1000 * ktvs[1].tv_usec;
	}

	return do_utimes(AT_FDCWD, filename, tvs ? tv : NULL, 0);
}
