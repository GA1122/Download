int sched_cpu_dying(unsigned int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

	 
	sched_ttwu_pending();
	raw_spin_lock_irqsave(&rq->lock, flags);
	if (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_offline(rq);
	}
	migrate_tasks(rq);
	BUG_ON(rq->nr_running != 1);
	raw_spin_unlock_irqrestore(&rq->lock, flags);
	calc_load_migrate(rq);
	update_max_interval();
	nohz_balance_exit_idle(cpu);
	hrtick_clear(rq);
	return 0;
}