static int slab_mem_going_online_callback(void *arg)
{
	struct kmem_cache_node *n;
	struct kmem_cache *s;
	struct memory_notify *marg = arg;
	int nid = marg->status_change_nid;
	int ret = 0;

	 
	if (nid < 0)
		return 0;

	 
	down_read(&slub_lock);
	list_for_each_entry(s, &slab_caches, list) {
		 
		n = kmem_cache_alloc(kmalloc_caches, GFP_KERNEL);
		if (!n) {
			ret = -ENOMEM;
			goto out;
		}
		init_kmem_cache_node(n);
		s->node[nid] = n;
	}
out:
	up_read(&slub_lock);
	return ret;
}
