static void ttwu_do_wakeup(struct rq *rq, struct task_struct *p, int wake_flags,
			   struct pin_cookie cookie)
{
	check_preempt_curr(rq, p, wake_flags);
	p->state = TASK_RUNNING;
	trace_sched_wakeup(p);

#ifdef CONFIG_SMP
	if (p->sched_class->task_woken) {
		 
		lockdep_unpin_lock(&rq->lock, cookie);
		p->sched_class->task_woken(rq, p);
		lockdep_repin_lock(&rq->lock, cookie);
	}

	if (rq->idle_stamp) {
		u64 delta = rq_clock(rq) - rq->idle_stamp;
		u64 max = 2*rq->max_idle_balance_cost;

		update_avg(&rq->avg_idle, delta);

		if (rq->avg_idle > max)
			rq->avg_idle = max;

		rq->idle_stamp = 0;
	}
#endif
}