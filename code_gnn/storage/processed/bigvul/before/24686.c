void posix_cpu_timer_schedule(struct k_itimer *timer)
{
	struct task_struct *p = timer->it.cpu.task;
	union cpu_time_count now;

	if (unlikely(p == NULL))
		 
		goto out;

	 
	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		cpu_clock_sample(timer->it_clock, p, &now);
		bump_cpu_timer(timer, now);
		if (unlikely(p->exit_state)) {
			clear_dead_task(timer, now);
			goto out;
		}
		read_lock(&tasklist_lock);  
	} else {
		read_lock(&tasklist_lock);
		if (unlikely(p->signal == NULL)) {
			 
			put_task_struct(p);
			timer->it.cpu.task = p = NULL;
			timer->it.cpu.expires.sched = 0;
			goto out_unlock;
		} else if (unlikely(p->exit_state) && thread_group_empty(p)) {
			 
			clear_dead_task(timer, now);
			goto out_unlock;
		}
		cpu_clock_sample_group(timer->it_clock, p, &now);
		bump_cpu_timer(timer, now);
		 
	}

	 
	arm_timer(timer, now);

out_unlock:
	read_unlock(&tasklist_lock);

out:
	timer->it_overrun_last = timer->it_overrun;
	timer->it_overrun = -1;
	++timer->it_requeue_pending;
}
