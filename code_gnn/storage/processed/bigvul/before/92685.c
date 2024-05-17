static int should_we_balance(struct lb_env *env)
{
	struct sched_group *sg = env->sd->groups;
	int cpu, balance_cpu = -1;

	 
	if (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		return 0;

	 
	if (env->idle == CPU_NEWLY_IDLE)
		return 1;

	 
	for_each_cpu_and(cpu, group_balance_mask(sg), env->cpus) {
		if (!idle_cpu(cpu))
			continue;

		balance_cpu = cpu;
		break;
	}

	if (balance_cpu == -1)
		balance_cpu = group_balance_cpu(sg);

	 
	return balance_cpu == env->dst_cpu;
}
