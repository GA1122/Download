static int leaks_show(struct seq_file *m, void *p)
{
	struct kmem_cache *cachep = list_entry(p, struct kmem_cache, list);
	struct page *page;
	struct kmem_cache_node *n;
	const char *name;
	unsigned long *x = m->private;
	int node;
	int i;

	if (!(cachep->flags & SLAB_STORE_USER))
		return 0;
	if (!(cachep->flags & SLAB_RED_ZONE))
		return 0;

	 
	do {
		set_store_user_clean(cachep);
		drain_cpu_caches(cachep);

		x[1] = 0;

		for_each_kmem_cache_node(cachep, node, n) {

			check_irq_on();
			spin_lock_irq(&n->list_lock);

			list_for_each_entry(page, &n->slabs_full, lru)
				handle_slab(x, cachep, page);
			list_for_each_entry(page, &n->slabs_partial, lru)
				handle_slab(x, cachep, page);
			spin_unlock_irq(&n->list_lock);
		}
	} while (!is_store_user_clean(cachep));

	name = cachep->name;
	if (x[0] == x[1]) {
		 
		mutex_unlock(&slab_mutex);
		m->private = kzalloc(x[0] * 4 * sizeof(unsigned long), GFP_KERNEL);
		if (!m->private) {
			 
			m->private = x;
			mutex_lock(&slab_mutex);
			return -ENOMEM;
		}
		*(unsigned long *)m->private = x[0] * 2;
		kfree(x);
		mutex_lock(&slab_mutex);
		 
		m->count = m->size;
		return 0;
	}
	for (i = 0; i < x[1]; i++) {
		seq_printf(m, "%s: %lu ", name, x[2*i+3]);
		show_symbol(m, x[2*i+2]);
		seq_putc(m, '\n');
	}

	return 0;
}