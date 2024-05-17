static void drain_array(struct kmem_cache *cachep, struct kmem_cache_node *n,
			 struct array_cache *ac, int node)
{
	LIST_HEAD(list);

	 
	check_mutex_acquired();

	if (!ac || !ac->avail)
		return;

	if (ac->touched) {
		ac->touched = 0;
		return;
	}

	spin_lock_irq(&n->list_lock);
	drain_array_locked(cachep, ac, node, false, &list);
	spin_unlock_irq(&n->list_lock);

	slabs_destroy(cachep, &list);
}