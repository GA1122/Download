static void init_numa_sched_groups_power(struct sched_group *group_head)
{
	struct sched_group *sg = group_head;
	int j;

	if (!sg)
		return;
	do {
		for_each_cpu(j, sched_group_cpus(sg)) {
			struct sched_domain *sd;

			sd = &per_cpu(phys_domains, j).sd;
			if (j != group_first_cpu(sd->groups)) {
				 
				continue;
			}

			sg->cpu_power += sd->groups->cpu_power;
		}
		sg = sg->next;
	} while (sg != group_head);
}