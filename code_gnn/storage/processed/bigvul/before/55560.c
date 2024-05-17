static int migration_cpu_stop(void *data)
{
	struct migration_arg *arg = data;
	struct task_struct *p = arg->task;
	struct rq *rq = this_rq();

	 
	local_irq_disable();
	 
	sched_ttwu_pending();

	raw_spin_lock(&p->pi_lock);
	raw_spin_lock(&rq->lock);
	 
	if (task_rq(p) == rq && task_on_rq_queued(p))
		rq = __migrate_task(rq, p, arg->dest_cpu);
	raw_spin_unlock(&rq->lock);
	raw_spin_unlock(&p->pi_lock);

	local_irq_enable();
	return 0;
}
