static int init_cache_node(struct kmem_cache *cachep, int node, gfp_t gfp)
{
	struct kmem_cache_node *n;

	 
	n = get_node(cachep, node);
	if (n) {
		spin_lock_irq(&n->list_lock);
		n->free_limit = (1 + nr_cpus_node(node)) * cachep->batchcount +
				cachep->num;
		spin_unlock_irq(&n->list_lock);

		return 0;
	}

	n = kmalloc_node(sizeof(struct kmem_cache_node), gfp, node);
	if (!n)
		return -ENOMEM;

	kmem_cache_node_init(n);
	n->next_reap = jiffies + REAPTIMEOUT_NODE +
		    ((unsigned long)cachep) % REAPTIMEOUT_NODE;

	n->free_limit =
		(1 + nr_cpus_node(node)) * cachep->batchcount + cachep->num;

	 
	cachep->node[node] = n;

	return 0;
}