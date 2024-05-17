static const struct cpumask *cpu_node_mask(int cpu)
{
	lockdep_assert_held(&sched_domains_mutex);

	sched_domain_node_span(cpu_to_node(cpu), sched_domains_tmpmask);

	return sched_domains_tmpmask;
}
