static int __sched_setscheduler(struct task_struct *p,
				const struct sched_attr *attr,
				bool user, bool pi)
{
	int newprio = dl_policy(attr->sched_policy) ? MAX_DL_PRIO - 1 :
		      MAX_RT_PRIO - 1 - attr->sched_priority;
	int retval, oldprio, oldpolicy = -1, queued, running;
	int new_effective_prio, policy = attr->sched_policy;
	const struct sched_class *prev_class;
	struct rq_flags rf;
	int reset_on_fork;
	int queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE;
	struct rq *rq;

	 
	BUG_ON(in_interrupt());
recheck:
	 
	if (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} else {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		if (!valid_policy(policy))
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
			if (attr->sched_nice < task_nice(p) &&
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

		  
		if (dl_policy(policy))
			return -EPERM;

		 
		if (idle_policy(p->policy) && !idle_policy(policy)) {
			if (!can_nice(p, task_nice(p)))
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

	 
	rq = task_rq_lock(p, &rf);

	 
	if (p == rq->stop) {
		task_rq_unlock(rq, p, &rf);
		return -EINVAL;
	}

	 
	if (unlikely(policy == p->policy)) {
		if (fair_policy(policy) && attr->sched_nice != task_nice(p))
			goto change;
		if (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			goto change;
		if (dl_policy(policy) && dl_param_changed(p, attr))
			goto change;

		p->sched_reset_on_fork = reset_on_fork;
		task_rq_unlock(rq, p, &rf);
		return 0;
	}
change:

	if (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		 
		if (rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			task_rq_unlock(rq, p, &rf);
			return -EPERM;
		}
#endif
#ifdef CONFIG_SMP
		if (dl_bandwidth_enabled() && dl_policy(policy)) {
			cpumask_t *span = rq->rd->span;

			 
			if (!cpumask_subset(span, &p->cpus_allowed) ||
			    rq->rd->dl_bw.bw == 0) {
				task_rq_unlock(rq, p, &rf);
				return -EPERM;
			}
		}
#endif
	}

	 
	if (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		goto recheck;
	}

	 
	if ((dl_policy(policy) || dl_task(p)) && dl_overflow(p, policy, attr)) {
		task_rq_unlock(rq, p, &rf);
		return -EBUSY;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	if (pi) {
		 
		new_effective_prio = rt_mutex_get_effective_prio(p, newprio);
		if (new_effective_prio == oldprio)
			queue_flags &= ~DEQUEUE_MOVE;
	}

	queued = task_on_rq_queued(p);
	running = task_current(rq, p);
	if (queued)
		dequeue_task(rq, p, queue_flags);
	if (running)
		put_prev_task(rq, p);

	prev_class = p->sched_class;
	__setscheduler(rq, p, attr, pi);

	if (running)
		p->sched_class->set_curr_task(rq);
	if (queued) {
		 
		if (oldprio < p->prio)
			queue_flags |= ENQUEUE_HEAD;

		enqueue_task(rq, p, queue_flags);
	}

	check_class_changed(rq, p, prev_class, oldprio);
	preempt_disable();  
	task_rq_unlock(rq, p, &rf);

	if (pi)
		rt_mutex_adjust_pi(p);

	 
	balance_callback(rq);
	preempt_enable();

	return 0;
}
