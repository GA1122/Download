int fib_unmerge(struct net *net)
{
	struct fib_table *old, *new;

	 
	old = fib_get_table(net, RT_TABLE_LOCAL);
	if (!old)
		return 0;

	new = fib_trie_unmerge(old);
	if (!new)
		return -ENOMEM;

	 
	if (new != old) {
		fib_replace_table(net, old, new);
		fib_free_table(old);
	}

	return 0;
}