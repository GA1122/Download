static void claim_allocations(int cpu, struct sched_domain *sd)
{
	struct sd_data *sdd = sd->private;
	struct sched_group *sg = sd->groups;

	WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
	*per_cpu_ptr(sdd->sd, cpu) = NULL;

	if (cpu == cpumask_first(sched_group_cpus(sg))) {
		WARN_ON_ONCE(*per_cpu_ptr(sdd->sg, cpu) != sg);
		*per_cpu_ptr(sdd->sg, cpu) = NULL;
	}
}
