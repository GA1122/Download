static int do_rt_sigqueueinfo(pid_t pid, int sig, siginfo_t *info)
{
	 
	if ((info->si_code >= 0 || info->si_code == SI_TKILL) &&
	    (task_pid_vnr(current) != pid))
		return -EPERM;

	info->si_signo = sig;

	 
	return kill_proc_info(sig, info, pid);
}
