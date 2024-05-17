unsigned long wait_task_inactive(struct task_struct *p, long match_state)
{
	int running, queued;
	struct rq_flags rf;
	unsigned long ncsw;
	struct rq *rq;

	for (;;) {
		 
		rq = task_rq(p);

		 
		while (task_running(rq, p)) {
			if (match_state && unlikely(p->state != match_state))
				return 0;
			cpu_relax();
		}

		 
		rq = task_rq_lock(p, &rf);
		trace_sched_wait_task(p);
		running = task_running(rq, p);
		queued = task_on_rq_queued(p);
		ncsw = 0;
		if (!match_state || p->state == match_state)
			ncsw = p->nvcsw | LONG_MIN;  
		task_rq_unlock(rq, p, &rf);

		 
		if (unlikely(!ncsw))
			break;

		 
		if (unlikely(running)) {
			cpu_relax();
			continue;
		}

		 
		if (unlikely(queued)) {
			ktime_t to = ktime_set(0, NSEC_PER_SEC/HZ);

			set_current_state(TASK_UNINTERRUPTIBLE);
			schedule_hrtimeout(&to, HRTIMER_MODE_REL);
			continue;
		}

		 
		break;
	}

	return ncsw;
}
