void sched_exec(void)
{
	struct task_struct *p = current;
	unsigned long flags;
	struct rq *rq;
	int dest_cpu;

	rq = task_rq_lock(p, &flags);
	dest_cpu = p->sched_class->select_task_rq(rq, p, SD_BALANCE_EXEC, 0);
	if (dest_cpu == smp_processor_id())
		goto unlock;

	 
	if (cpumask_test_cpu(dest_cpu, &p->cpus_allowed) &&
	    likely(cpu_active(dest_cpu)) && migrate_task(p, dest_cpu)) {
		struct migration_arg arg = { p, dest_cpu };

		task_rq_unlock(rq, &flags);
		stop_one_cpu(cpu_of(rq), migration_cpu_stop, &arg);
		return;
	}
unlock:
	task_rq_unlock(rq, &flags);
}