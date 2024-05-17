static void __sdt_free(const struct cpumask *cpu_map)
{
	struct sched_domain_topology_level *tl;
	int j;

	for (tl = sched_domain_topology; tl->init; tl++) {
		struct sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			kfree(*per_cpu_ptr(sdd->sd, j));
			kfree(*per_cpu_ptr(sdd->sg, j));
		}
		free_percpu(sdd->sd);
		free_percpu(sdd->sg);
	}
}
