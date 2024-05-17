static inline void ttwu_post_activation(struct task_struct *p, struct rq *rq,
					int wake_flags, bool success)
{
	trace_sched_wakeup(p, success);
	check_preempt_curr(rq, p, wake_flags);

	p->state = TASK_RUNNING;
#ifdef CONFIG_SMP
	if (p->sched_class->task_woken)
		p->sched_class->task_woken(rq, p);

	if (unlikely(rq->idle_stamp)) {
		u64 delta = rq->clock - rq->idle_stamp;
		u64 max = 2*sysctl_sched_migration_cost;

		if (delta > max)
			rq->avg_idle = max;
		else
			update_avg(&rq->avg_idle, delta);
		rq->idle_stamp = 0;
	}
#endif
	 
	if ((p->flags & PF_WQ_WORKER) && success)
		wq_worker_waking_up(p, cpu_of(rq));
}