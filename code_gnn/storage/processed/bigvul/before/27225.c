SYSCALL_DEFINE3(osf_statfs, const char __user *, pathname,
		struct osf_statfs __user *, buffer, unsigned long, bufsiz)
{
	struct kstatfs linux_stat;
	int error = user_statfs(pathname, &linux_stat);
	if (!error)
		error = linux_to_osf_statfs(&linux_stat, buffer, bufsiz);
	return error;	
}
