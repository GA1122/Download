static void init_sched_groups_power(int cpu, struct sched_domain *sd)
{
	WARN_ON(!sd || !sd->groups);

	if (cpu != group_first_cpu(sd->groups))
		return;

	sd->groups->group_weight = cpumask_weight(sched_group_cpus(sd->groups));

	update_group_power(sd, cpu);
}
