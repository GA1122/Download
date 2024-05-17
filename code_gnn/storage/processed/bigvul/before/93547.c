static void __net_exit ip6mr_rules_exit(struct net *net)
{
	struct mr6_table *mrt, *next;

	rtnl_lock();
	list_for_each_entry_safe(mrt, next, &net->ipv6.mr6_tables, list) {
		list_del(&mrt->list);
		ip6mr_free_table(mrt);
	}
	fib_rules_unregister(net->ipv6.mr6_rules_ops);
	rtnl_unlock();
}
