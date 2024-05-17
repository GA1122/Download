static void free_kmem_cache_nodes(struct kmem_cache *s)
{
	int node;

	for_each_node_state(node, N_NORMAL_MEMORY) {
		struct kmem_cache_node *n = s->node[node];
		if (n && n != &s->local_node)
			kmem_cache_free(kmalloc_caches, n);
		s->node[node] = NULL;
	}
}
