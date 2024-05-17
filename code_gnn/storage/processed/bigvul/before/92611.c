void nohz_balance_enter_idle(int cpu)
{
	struct rq *rq = cpu_rq(cpu);

	SCHED_WARN_ON(cpu != smp_processor_id());

	 
	if (!cpu_active(cpu))
		return;

	 
	if (!housekeeping_cpu(cpu, HK_FLAG_SCHED))
		return;

	 
	rq->has_blocked_load = 1;

	 
	if (rq->nohz_tick_stopped)
		goto out;

	 
	if (on_null_domain(rq))
		return;

	rq->nohz_tick_stopped = 1;

	cpumask_set_cpu(cpu, nohz.idle_cpus_mask);
	atomic_inc(&nohz.nr_cpus);

	 
	smp_mb__after_atomic();

	set_cpu_sd_state_idle(cpu);

out:
	 
	WRITE_ONCE(nohz.has_blocked, 1);
}
