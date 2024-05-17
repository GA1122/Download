svc_pool_map_choose_mode(void)
{
	unsigned int node;

	if (nr_online_nodes > 1) {
		 
		return SVC_POOL_PERNODE;
	}

	node = first_online_node;
	if (nr_cpus_node(node) > 2) {
		 
		return SVC_POOL_PERCPU;
	}

	 
	return SVC_POOL_GLOBAL;
}
