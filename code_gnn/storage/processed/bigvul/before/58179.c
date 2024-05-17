void rt_mutex_setprio(struct task_struct *p, int prio)
{
	int oldprio, on_rq, running, enqueue_flag = 0;
	struct rq *rq;
	const struct sched_class *prev_class;

	BUG_ON(prio > MAX_PRIO);

	rq = __task_rq_lock(p);

	 
	if (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		goto out_unlock;
	}

	trace_sched_pi_setprio(p, prio);
	p->pi_top_task = rt_mutex_get_top_task(p);
	oldprio = p->prio;
	prev_class = p->sched_class;
	on_rq = p->on_rq;
	running = task_current(rq, p);
	if (on_rq)
		dequeue_task(rq, p, 0);
	if (running)
		p->sched_class->put_prev_task(rq, p);

	 
	if (dl_prio(prio)) {
		if (!dl_prio(p->normal_prio) || (p->pi_top_task &&
			dl_entity_preempt(&p->pi_top_task->dl, &p->dl))) {
			p->dl.dl_boosted = 1;
			p->dl.dl_throttled = 0;
			enqueue_flag = ENQUEUE_REPLENISH;
		} else
			p->dl.dl_boosted = 0;
		p->sched_class = &dl_sched_class;
	} else if (rt_prio(prio)) {
		if (dl_prio(oldprio))
			p->dl.dl_boosted = 0;
		if (oldprio < prio)
			enqueue_flag = ENQUEUE_HEAD;
		p->sched_class = &rt_sched_class;
	} else {
		if (dl_prio(oldprio))
			p->dl.dl_boosted = 0;
		p->sched_class = &fair_sched_class;
	}

	p->prio = prio;

	if (running)
		p->sched_class->set_curr_task(rq);
	if (on_rq)
		enqueue_task(rq, p, enqueue_flag);

	check_class_changed(rq, p, prev_class, oldprio);
out_unlock:
	__task_rq_unlock(rq);
}
