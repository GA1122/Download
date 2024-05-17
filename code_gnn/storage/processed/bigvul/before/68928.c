static int setup_kmem_cache_nodes(struct kmem_cache *cachep, gfp_t gfp)
{
	int ret;
	int node;
	struct kmem_cache_node *n;

	for_each_online_node(node) {
		ret = setup_kmem_cache_node(cachep, node, gfp, true);
		if (ret)
			goto fail;

	}

	return 0;

fail:
	if (!cachep->list.next) {
		 
		node--;
		while (node >= 0) {
			n = get_node(cachep, node);
			if (n) {
				kfree(n->shared);
				free_alien_cache(n->alien);
				kfree(n);
				cachep->node[node] = NULL;
			}
			node--;
		}
	}
	return -ENOMEM;
}