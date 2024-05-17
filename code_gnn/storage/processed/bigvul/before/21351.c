static int shared_policy_replace(struct shared_policy *sp, unsigned long start,
				 unsigned long end, struct sp_node *new)
{
	struct sp_node *n, *new2 = NULL;

restart:
	spin_lock(&sp->lock);
	n = sp_lookup(sp, start, end);
	 
	while (n && n->start < end) {
		struct rb_node *next = rb_next(&n->nd);
		if (n->start >= start) {
			if (n->end <= end)
				sp_delete(sp, n);
			else
				n->start = end;
		} else {
			 
			if (n->end > end) {
				if (!new2) {
					spin_unlock(&sp->lock);
					new2 = sp_alloc(end, n->end, n->policy);
					if (!new2)
						return -ENOMEM;
					goto restart;
				}
				n->end = start;
				sp_insert(sp, new2);
				new2 = NULL;
				break;
			} else
				n->end = start;
		}
		if (!next)
			break;
		n = rb_entry(next, struct sp_node, nd);
	}
	if (new)
		sp_insert(sp, new);
	spin_unlock(&sp->lock);
	if (new2) {
		mpol_put(new2->policy);
		kmem_cache_free(sn_cache, new2);
	}
	return 0;
}
