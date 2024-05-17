static int select_fallback_rq(int cpu, struct task_struct *p)
{
	int dest_cpu;
	const struct cpumask *nodemask = cpumask_of_node(cpu_to_node(cpu));

	 
	for_each_cpu_and(dest_cpu, nodemask, cpu_active_mask)
		if (cpumask_test_cpu(dest_cpu, &p->cpus_allowed))
			return dest_cpu;

	 
	dest_cpu = cpumask_any_and(&p->cpus_allowed, cpu_active_mask);
	if (dest_cpu < nr_cpu_ids)
		return dest_cpu;

	 
	dest_cpu = cpuset_cpus_allowed_fallback(p);
	 
	if (p->mm && printk_ratelimit()) {
		printk(KERN_INFO "process %d (%s) no longer affine to cpu%d\n",
				task_pid_nr(p), p->comm, cpu);
	}

	return dest_cpu;
}
