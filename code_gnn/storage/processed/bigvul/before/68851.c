static void cache_reap(struct work_struct *w)
{
	struct kmem_cache *searchp;
	struct kmem_cache_node *n;
	int node = numa_mem_id();
	struct delayed_work *work = to_delayed_work(w);

	if (!mutex_trylock(&slab_mutex))
		 
		goto out;

	list_for_each_entry(searchp, &slab_caches, list) {
		check_irq_on();

		 
		n = get_node(searchp, node);

		reap_alien(searchp, n);

		drain_array(searchp, n, cpu_cache_get(searchp), node);

		 
		if (time_after(n->next_reap, jiffies))
			goto next;

		n->next_reap = jiffies + REAPTIMEOUT_NODE;

		drain_array(searchp, n, n->shared, node);

		if (n->free_touched)
			n->free_touched = 0;
		else {
			int freed;

			freed = drain_freelist(searchp, n, (n->free_limit +
				5 * searchp->num - 1) / (5 * searchp->num));
			STATS_ADD_REAPED(searchp, freed);
		}
next:
		cond_resched();
	}
	check_irq_on();
	mutex_unlock(&slab_mutex);
	next_reap_node();
out:
	 
	schedule_delayed_work(work, round_jiffies_relative(REAPTIMEOUT_AC));
}
