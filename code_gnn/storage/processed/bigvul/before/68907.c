void __init kmem_cache_init_late(void)
{
	struct kmem_cache *cachep;

	slab_state = UP;

	 
	mutex_lock(&slab_mutex);
	list_for_each_entry(cachep, &slab_caches, list)
		if (enable_cpucache(cachep, GFP_NOWAIT))
			BUG();
	mutex_unlock(&slab_mutex);

	 
	slab_state = FULL;

#ifdef CONFIG_NUMA
	 
	hotplug_memory_notifier(slab_memory_callback, SLAB_CALLBACK_PRI);
#endif

	 
}
