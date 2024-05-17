static void mem_cgroup_may_update_nodemask(struct mem_cgroup *memcg)
{
	int nid;
	 
	if (!atomic_read(&memcg->numainfo_events))
		return;
	if (atomic_inc_return(&memcg->numainfo_updating) > 1)
		return;

	 
	memcg->scan_nodes = node_states[N_HIGH_MEMORY];

	for_each_node_mask(nid, node_states[N_HIGH_MEMORY]) {

		if (!test_mem_cgroup_node_reclaimable(memcg, nid, false))
			node_clear(nid, memcg->scan_nodes);
	}

	atomic_set(&memcg->numainfo_events, 0);
	atomic_set(&memcg->numainfo_updating, 0);
}
