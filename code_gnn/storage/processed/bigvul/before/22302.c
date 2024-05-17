static void __free_domain_allocs(struct s_data *d, enum s_alloc what,
				 const struct cpumask *cpu_map)
{
	switch (what) {
	case sa_sched_groups:
		free_sched_groups(cpu_map, d->tmpmask);  
		d->sched_group_nodes = NULL;
	case sa_rootdomain:
		free_rootdomain(d->rd);  
	case sa_tmpmask:
		free_cpumask_var(d->tmpmask);  
	case sa_send_covered:
		free_cpumask_var(d->send_covered);  
	case sa_this_book_map:
		free_cpumask_var(d->this_book_map);  
	case sa_this_core_map:
		free_cpumask_var(d->this_core_map);  
	case sa_this_sibling_map:
		free_cpumask_var(d->this_sibling_map);  
	case sa_nodemask:
		free_cpumask_var(d->nodemask);  
	case sa_sched_group_nodes:
#ifdef CONFIG_NUMA
		kfree(d->sched_group_nodes);  
	case sa_notcovered:
		free_cpumask_var(d->notcovered);  
	case sa_covered:
		free_cpumask_var(d->covered);  
	case sa_domainspan:
		free_cpumask_var(d->domainspan);  
#endif
	case sa_none:
		break;
	}
}