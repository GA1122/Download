static void mem_cgroup_drain_pcp_counter(struct mem_cgroup *memcg, int cpu)
{
	int i;

	spin_lock(&memcg->pcp_counter_lock);
	for (i = 0; i < MEM_CGROUP_STAT_DATA; i++) {
		long x = per_cpu(memcg->stat->count[i], cpu);

		per_cpu(memcg->stat->count[i], cpu) = 0;
		memcg->nocpu_base.count[i] += x;
	}
	for (i = 0; i < MEM_CGROUP_EVENTS_NSTATS; i++) {
		unsigned long x = per_cpu(memcg->stat->events[i], cpu);

		per_cpu(memcg->stat->events[i], cpu) = 0;
		memcg->nocpu_base.events[i] += x;
	}
	 
	per_cpu(memcg->stat->count[MEM_CGROUP_ON_MOVE], cpu) = 0;
	spin_unlock(&memcg->pcp_counter_lock);
}