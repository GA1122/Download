static unsigned long score_nearby_nodes(struct task_struct *p, int nid,
					int maxdist, bool task)
{
	unsigned long score = 0;
	int node;

	 
	if (sched_numa_topology_type == NUMA_DIRECT)
		return 0;

	 
	for_each_online_node(node) {
		unsigned long faults;
		int dist = node_distance(nid, node);

		 
		if (dist == sched_max_numa_distance || node == nid)
			continue;

		 
		if (sched_numa_topology_type == NUMA_BACKPLANE &&
					dist >= maxdist)
			continue;

		 
		if (task)
			faults = task_faults(p, node);
		else
			faults = group_faults(p, node);

		 
		if (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			faults *= (sched_max_numa_distance - dist);
			faults /= (sched_max_numa_distance - LOCAL_DISTANCE);
		}

		score += faults;
	}

	return score;
}
