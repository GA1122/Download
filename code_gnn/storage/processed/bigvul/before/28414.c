static void fib6_link_table(struct net *net, struct fib6_table *tb)
{
	unsigned int h;

	 
	rwlock_init(&tb->tb6_lock);

	h = tb->tb6_id & (FIB6_TABLE_HASHSZ - 1);

	 
	hlist_add_head_rcu(&tb->tb6_hlist, &net->ipv6.fib_table_hash[h]);
}
