int select_task_rq(struct task_struct *p, int cpu, int sd_flags, int wake_flags)
{
	cpu = p->sched_class->select_task_rq(p, cpu, sd_flags, wake_flags);

	 
	if (unlikely(!cpumask_test_cpu(cpu, tsk_cpus_allowed(p)) ||
		     !cpu_online(cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	return cpu;
}
