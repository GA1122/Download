static int cpuup_prepare(long cpu)
{
	struct kmem_cache *cachep;
	int node = cpu_to_mem(cpu);
	int err;

	 
	err = init_cache_node_node(node);
	if (err < 0)
		goto bad;

	 
	list_for_each_entry(cachep, &slab_caches, list) {
		err = setup_kmem_cache_node(cachep, node, GFP_KERNEL, false);
		if (err)
			goto bad;
	}

	return 0;
bad:
	cpuup_canceled(cpu);
	return -ENOMEM;
}
