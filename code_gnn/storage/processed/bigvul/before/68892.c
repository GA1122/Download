static void __init init_list(struct kmem_cache *cachep, struct kmem_cache_node *list,
				int nodeid)
{
	struct kmem_cache_node *ptr;

	ptr = kmalloc_node(sizeof(struct kmem_cache_node), GFP_NOWAIT, nodeid);
	BUG_ON(!ptr);

	memcpy(ptr, list, sizeof(struct kmem_cache_node));
	 
	spin_lock_init(&ptr->list_lock);

	MAKE_ALL_LISTS(cachep, ptr, nodeid);
	cachep->node[nodeid] = ptr;
}