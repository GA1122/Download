int posix_cpu_clock_get(const clockid_t which_clock, struct timespec *tp)
{
	const pid_t pid = CPUCLOCK_PID(which_clock);
	int error = -EINVAL;
	union cpu_time_count rtn;

	if (pid == 0) {
		 
		if (CPUCLOCK_PERTHREAD(which_clock)) {
			 
			error = cpu_clock_sample(which_clock,
						 current, &rtn);
		} else {
			read_lock(&tasklist_lock);
			error = cpu_clock_sample_group(which_clock,
						       current, &rtn);
			read_unlock(&tasklist_lock);
		}
	} else {
		 
		struct task_struct *p;
		rcu_read_lock();
		p = find_task_by_vpid(pid);
		if (p) {
			if (CPUCLOCK_PERTHREAD(which_clock)) {
				if (same_thread_group(p, current)) {
					error = cpu_clock_sample(which_clock,
								 p, &rtn);
				}
			} else {
				read_lock(&tasklist_lock);
				if (thread_group_leader(p) && p->signal) {
					error =
					    cpu_clock_sample_group(which_clock,
							           p, &rtn);
				}
				read_unlock(&tasklist_lock);
			}
		}
		rcu_read_unlock();
	}

	if (error)
		return error;
	sample_to_timespec(which_clock, rtn, tp);
	return 0;
}
