static int find_next_best_node(int node, nodemask_t *used_nodes)
{
	int i, n, val, min_val, best_node = -1;

	min_val = INT_MAX;

	for (i = 0; i < nr_node_ids; i++) {
		 
		n = (node + i) % nr_node_ids;

		if (!nr_cpus_node(n))
			continue;

		 
		if (node_isset(n, *used_nodes))
			continue;

		 
		val = node_distance(node, n);

		if (val < min_val) {
			min_val = val;
			best_node = n;
		}
	}

	if (best_node != -1)
		node_set(best_node, *used_nodes);
	return best_node;
}
