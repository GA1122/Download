void set_user_nice(struct task_struct *p, long nice)
{
	int old_prio, delta, queued;
	struct rq_flags rf;
	struct rq *rq;

	if (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		return;
	 
	rq = task_rq_lock(p, &rf);
	 
	if (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		goto out_unlock;
	}
	queued = task_on_rq_queued(p);
	if (queued)
		dequeue_task(rq, p, DEQUEUE_SAVE);

	p->static_prio = NICE_TO_PRIO(nice);
	set_load_weight(p);
	old_prio = p->prio;
	p->prio = effective_prio(p);
	delta = p->prio - old_prio;

	if (queued) {
		enqueue_task(rq, p, ENQUEUE_RESTORE);
		 
		if (delta < 0 || (delta > 0 && task_running(rq, p)))
			resched_curr(rq);
	}
out_unlock:
	task_rq_unlock(rq, p, &rf);
}