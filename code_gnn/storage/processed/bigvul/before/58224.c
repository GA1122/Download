static int select_fallback_rq(int cpu, struct task_struct *p)
{
	int nid = cpu_to_node(cpu);
	const struct cpumask *nodemask = NULL;
	enum { cpuset, possible, fail } state = cpuset;
	int dest_cpu;

	 
	if (nid != -1) {
		nodemask = cpumask_of_node(nid);

		 
		for_each_cpu(dest_cpu, nodemask) {
			if (!cpu_online(dest_cpu))
				continue;
			if (!cpu_active(dest_cpu))
				continue;
			if (cpumask_test_cpu(dest_cpu, tsk_cpus_allowed(p)))
				return dest_cpu;
		}
	}

	for (;;) {
		 
		for_each_cpu(dest_cpu, tsk_cpus_allowed(p)) {
			if (!cpu_online(dest_cpu))
				continue;
			if (!cpu_active(dest_cpu))
				continue;
			goto out;
		}

		switch (state) {
		case cpuset:
			 
			cpuset_cpus_allowed_fallback(p);
			state = possible;
			break;

		case possible:
			do_set_cpus_allowed(p, cpu_possible_mask);
			state = fail;
			break;

		case fail:
			BUG();
			break;
		}
	}

out:
	if (state != cpuset) {
		 
		if (p->mm && printk_ratelimit()) {
			printk_sched("process %d (%s) no longer affine to cpu%d\n",
					task_pid_nr(p), p->comm, cpu);
		}
	}

	return dest_cpu;
}
