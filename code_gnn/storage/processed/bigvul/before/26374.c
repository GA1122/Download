bool __sched yield_to(struct task_struct *p, bool preempt)
{
	struct task_struct *curr = current;
	struct rq *rq, *p_rq;
	unsigned long flags;
	bool yielded = 0;

	local_irq_save(flags);
	rq = this_rq();

again:
	p_rq = task_rq(p);
	double_rq_lock(rq, p_rq);
	while (task_rq(p) != p_rq) {
		double_rq_unlock(rq, p_rq);
		goto again;
	}

	if (!curr->sched_class->yield_to_task)
		goto out;

	if (curr->sched_class != p->sched_class)
		goto out;

	if (task_running(p_rq, p) || p->state)
		goto out;

	yielded = curr->sched_class->yield_to_task(rq, p, preempt);
	if (yielded) {
		schedstat_inc(rq, yld_count);
		 
		if (preempt && rq != p_rq)
			resched_task(p_rq->curr);
	}

out:
	double_rq_unlock(rq, p_rq);
	local_irq_restore(flags);

	if (yielded)
		schedule();

	return yielded;
}
