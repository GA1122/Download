int select_task_rq(struct task_struct *p, int cpu, int sd_flags, int wake_flags)
{
	lockdep_assert_held(&p->pi_lock);

	if (tsk_nr_cpus_allowed(p) > 1)
		cpu = p->sched_class->select_task_rq(p, cpu, sd_flags, wake_flags);
	else
		cpu = cpumask_any(tsk_cpus_allowed(p));

	 
	if (unlikely(!cpumask_test_cpu(cpu, tsk_cpus_allowed(p)) ||
		     !cpu_online(cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	return cpu;
}
