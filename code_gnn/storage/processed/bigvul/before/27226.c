SYSCALL_DEFINE3(osf_fstatfs, unsigned long, fd,
		struct osf_statfs __user *, buffer, unsigned long, bufsiz)
{
	struct kstatfs linux_stat;
	int error = fd_statfs(fd, &linux_stat);
	if (!error)
		error = linux_to_osf_statfs(&linux_stat, buffer, bufsiz);
	return error;
}
