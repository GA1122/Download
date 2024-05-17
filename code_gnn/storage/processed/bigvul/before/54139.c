static int __net_init ip_fib_net_init(struct net *net)
{
	int err;
	size_t size = sizeof(struct hlist_head) * FIB_TABLE_HASHSZ;

	 
	size = max_t(size_t, size, L1_CACHE_BYTES);

	net->ipv4.fib_table_hash = kzalloc(size, GFP_KERNEL);
	if (!net->ipv4.fib_table_hash)
		return -ENOMEM;

	err = fib4_rules_init(net);
	if (err < 0)
		goto fail;
	return 0;

fail:
	kfree(net->ipv4.fib_table_hash);
	return err;
}
