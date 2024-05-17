asmlinkage long sys_waitid(int which, pid_t upid,
			   struct siginfo __user *infop, int options,
			   struct rusage __user *ru)
{
	struct pid *pid = NULL;
	enum pid_type type;
	long ret;

	if (options & ~(WNOHANG|WNOWAIT|WEXITED|WSTOPPED|WCONTINUED))
		return -EINVAL;
	if (!(options & (WEXITED|WSTOPPED|WCONTINUED)))
		return -EINVAL;

	switch (which) {
	case P_ALL:
		type = PIDTYPE_MAX;
		break;
	case P_PID:
		type = PIDTYPE_PID;
		if (upid <= 0)
			return -EINVAL;
		break;
	case P_PGID:
		type = PIDTYPE_PGID;
		if (upid <= 0)
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}

	if (type < PIDTYPE_MAX)
		pid = find_get_pid(upid);
	ret = do_wait(type, pid, options, infop, NULL, ru);
	put_pid(pid);

	 
	asmlinkage_protect(5, ret, which, upid, infop, options, ru);
	return ret;
}