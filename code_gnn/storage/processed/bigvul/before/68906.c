void __init kmem_cache_init(void)
{
	int i;

	BUILD_BUG_ON(sizeof(((struct page *)NULL)->lru) <
					sizeof(struct rcu_head));
	kmem_cache = &kmem_cache_boot;

	if (!IS_ENABLED(CONFIG_NUMA) || num_possible_nodes() == 1)
		use_alien_caches = 0;

	for (i = 0; i < NUM_INIT_LISTS; i++)
		kmem_cache_node_init(&init_kmem_cache_node[i]);

	 
	if (!slab_max_order_set && totalram_pages > (32 << 20) >> PAGE_SHIFT)
		slab_max_order = SLAB_MAX_ORDER_HI;

	 

	 

	 
	create_boot_cache(kmem_cache, "kmem_cache",
		offsetof(struct kmem_cache, node) +
				  nr_node_ids * sizeof(struct kmem_cache_node *),
				  SLAB_HWCACHE_ALIGN);
	list_add(&kmem_cache->list, &slab_caches);
	slab_state = PARTIAL;

	 
	kmalloc_caches[INDEX_NODE] = create_kmalloc_cache("kmalloc-node",
				kmalloc_size(INDEX_NODE), ARCH_KMALLOC_FLAGS);
	slab_state = PARTIAL_NODE;
	setup_kmalloc_cache_index_table();

	slab_early_init = 0;

	 
	{
		int nid;

		for_each_online_node(nid) {
			init_list(kmem_cache, &init_kmem_cache_node[CACHE_CACHE + nid], nid);

			init_list(kmalloc_caches[INDEX_NODE],
					  &init_kmem_cache_node[SIZE_NODE + nid], nid);
		}
	}

	create_kmalloc_caches(ARCH_KMALLOC_FLAGS);
}
