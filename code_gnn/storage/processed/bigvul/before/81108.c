static int posix_cpu_clock_get(const clockid_t which_clock, struct timespec64 *tp)
{
	const pid_t pid = CPUCLOCK_PID(which_clock);
	int err = -EINVAL;

	if (pid == 0) {
		 
		err = posix_cpu_clock_get_task(current, which_clock, tp);
	} else {
		 
		struct task_struct *p;
		rcu_read_lock();
		p = find_task_by_vpid(pid);
		if (p)
			err = posix_cpu_clock_get_task(p, which_clock, tp);
		rcu_read_unlock();
	}

	return err;
}
