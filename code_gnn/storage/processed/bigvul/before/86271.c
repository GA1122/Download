static void cleanup_net(struct work_struct *work)
{
	const struct pernet_operations *ops;
	struct net *net, *tmp;
	struct list_head net_kill_list;
	LIST_HEAD(net_exit_list);

	 
	spin_lock_irq(&cleanup_list_lock);
	list_replace_init(&cleanup_list, &net_kill_list);
	spin_unlock_irq(&cleanup_list_lock);

	mutex_lock(&net_mutex);

	 
	rtnl_lock();
	list_for_each_entry(net, &net_kill_list, cleanup_list) {
		list_del_rcu(&net->list);
		list_add_tail(&net->exit_list, &net_exit_list);
		for_each_net(tmp) {
			int id;

			spin_lock_bh(&tmp->nsid_lock);
			id = __peernet2id(tmp, net);
			if (id >= 0)
				idr_remove(&tmp->netns_ids, id);
			spin_unlock_bh(&tmp->nsid_lock);
			if (id >= 0)
				rtnl_net_notifyid(tmp, RTM_DELNSID, id);
		}
		spin_lock_bh(&net->nsid_lock);
		idr_destroy(&net->netns_ids);
		spin_unlock_bh(&net->nsid_lock);

	}
	rtnl_unlock();

	 
	synchronize_rcu();

	 
	list_for_each_entry_reverse(ops, &pernet_list, list)
		ops_exit_list(ops, &net_exit_list);

	 
	list_for_each_entry_reverse(ops, &pernet_list, list)
		ops_free_list(ops, &net_exit_list);

	mutex_unlock(&net_mutex);

	 
	rcu_barrier();

	 
	list_for_each_entry_safe(net, tmp, &net_exit_list, exit_list) {
		list_del_init(&net->exit_list);
		dec_net_namespaces(net->ucounts);
		put_user_ns(net->user_ns);
		net_drop_ns(net);
	}
}
