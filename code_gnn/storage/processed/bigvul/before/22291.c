SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, int, policy,
		struct sched_param __user *, param)
{
	 
	if (policy < 0)
		return -EINVAL;

	return do_sched_setscheduler(pid, policy, param);
}
