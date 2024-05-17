SYSCALL_DEFINE0(fork)
{
#ifdef CONFIG_MMU
	return do_fork(SIGCHLD, 0, 0, NULL, NULL);
#else
	 
	return(-EINVAL);
#endif
}
