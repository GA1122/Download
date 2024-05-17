static void cleanup_net(struct work_struct *work)
{
	const struct pernet_operations *ops;
	struct net *net, *tmp, *last;
	struct llist_node *net_kill_list;
	LIST_HEAD(net_exit_list);

	 
	net_kill_list = llist_del_all(&cleanup_list);

	down_read(&pernet_ops_rwsem);

	 
	down_write(&net_rwsem);
	llist_for_each_entry(net, net_kill_list, cleanup_list)
		list_del_rcu(&net->list);
	 
	last = list_last_entry(&net_namespace_list, struct net, list);
	up_write(&net_rwsem);

	llist_for_each_entry(net, net_kill_list, cleanup_list) {
		unhash_nsid(net, last);
		list_add_tail(&net->exit_list, &net_exit_list);
	}

	 
	synchronize_rcu();

	 
	list_for_each_entry_reverse(ops, &pernet_list, list)
		ops_exit_list(ops, &net_exit_list);

	 
	list_for_each_entry_reverse(ops, &pernet_list, list)
		ops_free_list(ops, &net_exit_list);

	up_read(&pernet_ops_rwsem);

	 
	rcu_barrier();

	 
	list_for_each_entry_safe(net, tmp, &net_exit_list, exit_list) {
		list_del_init(&net->exit_list);
		dec_net_namespaces(net->ucounts);
		put_user_ns(net->user_ns);
		net_drop_ns(net);
	}
}
