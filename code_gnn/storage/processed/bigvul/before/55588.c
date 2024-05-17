int sched_cpu_activate(unsigned int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

	set_cpu_active(cpu, true);

	if (sched_smp_initialized) {
		sched_domains_numa_masks_set(cpu);
		cpuset_cpu_active();
	}

	 
	raw_spin_lock_irqsave(&rq->lock, flags);
	if (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_online(rq);
	}
	raw_spin_unlock_irqrestore(&rq->lock, flags);

	update_max_interval();

	return 0;
}
