static int __build_sched_domains(const struct cpumask *cpu_map,
				 struct sched_domain_attr *attr)
{
	enum s_alloc alloc_state = sa_none;
	struct s_data d;
	struct sched_domain *sd;
	int i;
#ifdef CONFIG_NUMA
	d.sd_allnodes = 0;
#endif

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	if (alloc_state != sa_rootdomain)
		goto error;
	alloc_state = sa_sched_groups;

	 
	for_each_cpu(i, cpu_map) {
		cpumask_and(d.nodemask, cpumask_of_node(cpu_to_node(i)),
			    cpu_map);

		sd = __build_numa_sched_domains(&d, cpu_map, attr, i);
		sd = __build_cpu_sched_domain(&d, cpu_map, attr, sd, i);
		sd = __build_book_sched_domain(&d, cpu_map, attr, sd, i);
		sd = __build_mc_sched_domain(&d, cpu_map, attr, sd, i);
		sd = __build_smt_sched_domain(&d, cpu_map, attr, sd, i);
	}

	for_each_cpu(i, cpu_map) {
		build_sched_groups(&d, SD_LV_SIBLING, cpu_map, i);
		build_sched_groups(&d, SD_LV_BOOK, cpu_map, i);
		build_sched_groups(&d, SD_LV_MC, cpu_map, i);
	}

	 
	for (i = 0; i < nr_node_ids; i++)
		build_sched_groups(&d, SD_LV_CPU, cpu_map, i);

#ifdef CONFIG_NUMA
	 
	if (d.sd_allnodes)
		build_sched_groups(&d, SD_LV_ALLNODES, cpu_map, 0);

	for (i = 0; i < nr_node_ids; i++)
		if (build_numa_sched_groups(&d, cpu_map, i))
			goto error;
#endif

	 
#ifdef CONFIG_SCHED_SMT
	for_each_cpu(i, cpu_map) {
		sd = &per_cpu(cpu_domains, i).sd;
		init_sched_groups_power(i, sd);
	}
#endif
#ifdef CONFIG_SCHED_MC
	for_each_cpu(i, cpu_map) {
		sd = &per_cpu(core_domains, i).sd;
		init_sched_groups_power(i, sd);
	}
#endif
#ifdef CONFIG_SCHED_BOOK
	for_each_cpu(i, cpu_map) {
		sd = &per_cpu(book_domains, i).sd;
		init_sched_groups_power(i, sd);
	}
#endif

	for_each_cpu(i, cpu_map) {
		sd = &per_cpu(phys_domains, i).sd;
		init_sched_groups_power(i, sd);
	}

#ifdef CONFIG_NUMA
	for (i = 0; i < nr_node_ids; i++)
		init_numa_sched_groups_power(d.sched_group_nodes[i]);

	if (d.sd_allnodes) {
		struct sched_group *sg;

		cpu_to_allnodes_group(cpumask_first(cpu_map), cpu_map, &sg,
								d.tmpmask);
		init_numa_sched_groups_power(sg);
	}
#endif

	 
	for_each_cpu(i, cpu_map) {
#ifdef CONFIG_SCHED_SMT
		sd = &per_cpu(cpu_domains, i).sd;
#elif defined(CONFIG_SCHED_MC)
		sd = &per_cpu(core_domains, i).sd;
#elif defined(CONFIG_SCHED_BOOK)
		sd = &per_cpu(book_domains, i).sd;
#else
		sd = &per_cpu(phys_domains, i).sd;
#endif
		cpu_attach_domain(sd, d.rd, i);
	}

	d.sched_group_nodes = NULL;  
	__free_domain_allocs(&d, sa_tmpmask, cpu_map);
	return 0;

error:
	__free_domain_allocs(&d, alloc_state, cpu_map);
	return -ENOMEM;
}