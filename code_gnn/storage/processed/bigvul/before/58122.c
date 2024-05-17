static int __sched_setscheduler(struct task_struct *p,
				const struct sched_attr *attr,
				bool user)
{
	int retval, oldprio, oldpolicy = -1, on_rq, running;
	int policy = attr->sched_policy;
	unsigned long flags;
	const struct sched_class *prev_class;
	struct rq *rq;
	int reset_on_fork;

	 
	BUG_ON(in_interrupt());
recheck:
	 
	if (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} else {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		if (policy != SCHED_DEADLINE &&
				policy != SCHED_FIFO && policy != SCHED_RR &&
				policy != SCHED_NORMAL && policy != SCHED_BATCH &&
				policy != SCHED_IDLE)
			return -EINVAL;
	}

	if (attr->sched_flags & ~(SCHED_FLAG_RESET_ON_FORK))
		return -EINVAL;

	 
	if ((p->mm && attr->sched_priority > MAX_USER_RT_PRIO-1) ||
	    (!p->mm && attr->sched_priority > MAX_RT_PRIO-1))
		return -EINVAL;
	if ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		return -EINVAL;

	 
	if (user && !capable(CAP_SYS_NICE)) {
		if (fair_policy(policy)) {
			if (attr->sched_nice < TASK_NICE(p) &&
			    !can_nice(p, attr->sched_nice))
				return -EPERM;
		}

		if (rt_policy(policy)) {
			unsigned long rlim_rtprio =
					task_rlimit(p, RLIMIT_RTPRIO);

			 
			if (policy != p->policy && !rlim_rtprio)
				return -EPERM;

			 
			if (attr->sched_priority > p->rt_priority &&
			    attr->sched_priority > rlim_rtprio)
				return -EPERM;
		}

		 
		if (p->policy == SCHED_IDLE && policy != SCHED_IDLE) {
			if (!can_nice(p, TASK_NICE(p)))
				return -EPERM;
		}

		 
		if (!check_same_owner(p))
			return -EPERM;

		 
		if (p->sched_reset_on_fork && !reset_on_fork)
			return -EPERM;
	}

	if (user) {
		retval = security_task_setscheduler(p);
		if (retval)
			return retval;
	}

	 
	rq = task_rq_lock(p, &flags);

	 
	if (p == rq->stop) {
		task_rq_unlock(rq, p, &flags);
		return -EINVAL;
	}

	 
	if (unlikely(policy == p->policy)) {
		if (fair_policy(policy) && attr->sched_nice != TASK_NICE(p))
			goto change;
		if (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			goto change;
		if (dl_policy(policy))
			goto change;

		task_rq_unlock(rq, p, &flags);
		return 0;
	}
change:

	if (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		 
		if (rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			task_rq_unlock(rq, p, &flags);
			return -EPERM;
		}
#endif
#ifdef CONFIG_SMP
		if (dl_bandwidth_enabled() && dl_policy(policy)) {
			cpumask_t *span = rq->rd->span;

			 
			if (!cpumask_subset(span, &p->cpus_allowed) ||
			    rq->rd->dl_bw.bw == 0) {
				task_rq_unlock(rq, p, &flags);
				return -EPERM;
			}
		}
#endif
	}

	 
	if (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &flags);
		goto recheck;
	}

	 
	if ((dl_policy(policy) || dl_task(p)) && dl_overflow(p, policy, attr)) {
		task_rq_unlock(rq, p, &flags);
		return -EBUSY;
	}

	on_rq = p->on_rq;
	running = task_current(rq, p);
	if (on_rq)
		dequeue_task(rq, p, 0);
	if (running)
		p->sched_class->put_prev_task(rq, p);

	p->sched_reset_on_fork = reset_on_fork;

	oldprio = p->prio;
	prev_class = p->sched_class;
	__setscheduler(rq, p, attr);

	if (running)
		p->sched_class->set_curr_task(rq);
	if (on_rq)
		enqueue_task(rq, p, 0);

	check_class_changed(rq, p, prev_class, oldprio);
	task_rq_unlock(rq, p, &flags);

	rt_mutex_adjust_pi(p);

	return 0;
}
