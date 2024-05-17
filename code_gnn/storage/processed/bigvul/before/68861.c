static void cpuup_canceled(long cpu)
{
	struct kmem_cache *cachep;
	struct kmem_cache_node *n = NULL;
	int node = cpu_to_mem(cpu);
	const struct cpumask *mask = cpumask_of_node(node);

	list_for_each_entry(cachep, &slab_caches, list) {
		struct array_cache *nc;
		struct array_cache *shared;
		struct alien_cache **alien;
		LIST_HEAD(list);

		n = get_node(cachep, node);
		if (!n)
			continue;

		spin_lock_irq(&n->list_lock);

		 
		n->free_limit -= cachep->batchcount;

		 
		nc = per_cpu_ptr(cachep->cpu_cache, cpu);
		if (nc) {
			free_block(cachep, nc->entry, nc->avail, node, &list);
			nc->avail = 0;
		}

		if (!cpumask_empty(mask)) {
			spin_unlock_irq(&n->list_lock);
			goto free_slab;
		}

		shared = n->shared;
		if (shared) {
			free_block(cachep, shared->entry,
				   shared->avail, node, &list);
			n->shared = NULL;
		}

		alien = n->alien;
		n->alien = NULL;

		spin_unlock_irq(&n->list_lock);

		kfree(shared);
		if (alien) {
			drain_alien_cache(cachep, alien);
			free_alien_cache(alien);
		}

free_slab:
		slabs_destroy(cachep, &list);
	}
	 
	list_for_each_entry(cachep, &slab_caches, list) {
		n = get_node(cachep, node);
		if (!n)
			continue;
		drain_freelist(cachep, n, INT_MAX);
	}
}