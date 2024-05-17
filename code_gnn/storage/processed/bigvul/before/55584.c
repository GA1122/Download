void rt_mutex_setprio(struct task_struct *p, int prio)
{
	int oldprio, queued, running, queue_flag = DEQUEUE_SAVE | DEQUEUE_MOVE;
	const struct sched_class *prev_class;
	struct rq_flags rf;
	struct rq *rq;

	BUG_ON(prio > MAX_PRIO);

	rq = __task_rq_lock(p, &rf);

	 
	if (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		goto out_unlock;
	}

	trace_sched_pi_setprio(p, prio);
	oldprio = p->prio;

	if (oldprio == prio)
		queue_flag &= ~DEQUEUE_MOVE;

	prev_class = p->sched_class;
	queued = task_on_rq_queued(p);
	running = task_current(rq, p);
	if (queued)
		dequeue_task(rq, p, queue_flag);
	if (running)
		put_prev_task(rq, p);

	 
	if (dl_prio(prio)) {
		struct task_struct *pi_task = rt_mutex_get_top_task(p);
		if (!dl_prio(p->normal_prio) ||
		    (pi_task && dl_entity_preempt(&pi_task->dl, &p->dl))) {
			p->dl.dl_boosted = 1;
			queue_flag |= ENQUEUE_REPLENISH;
		} else
			p->dl.dl_boosted = 0;
		p->sched_class = &dl_sched_class;
	} else if (rt_prio(prio)) {
		if (dl_prio(oldprio))
			p->dl.dl_boosted = 0;
		if (oldprio < prio)
			queue_flag |= ENQUEUE_HEAD;
		p->sched_class = &rt_sched_class;
	} else {
		if (dl_prio(oldprio))
			p->dl.dl_boosted = 0;
		if (rt_prio(oldprio))
			p->rt.timeout = 0;
		p->sched_class = &fair_sched_class;
	}

	p->prio = prio;

	if (running)
		p->sched_class->set_curr_task(rq);
	if (queued)
		enqueue_task(rq, p, queue_flag);

	check_class_changed(rq, p, prev_class, oldprio);
out_unlock:
	preempt_disable();  
	__task_rq_unlock(rq, &rf);

	balance_callback(rq);
	preempt_enable();
}
