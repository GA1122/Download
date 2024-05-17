unsigned long long task_sched_runtime(struct task_struct *p)
{
	unsigned long flags;
	struct rq *rq;
	u64 ns = 0;

#if defined(CONFIG_64BIT) && defined(CONFIG_SMP)
	 
	if (!p->on_cpu)
		return p->se.sum_exec_runtime;
#endif

	rq = task_rq_lock(p, &flags);
	ns = p->se.sum_exec_runtime + do_task_delta_exec(p, rq);
	task_rq_unlock(rq, p, &flags);

	return ns;
}
