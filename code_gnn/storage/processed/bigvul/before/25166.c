static void rt_do_flush(struct net *net, int process_context)
{
	unsigned int i;
	struct rtable *rth, *next;

	for (i = 0; i <= rt_hash_mask; i++) {
		struct rtable __rcu **pprev;
		struct rtable *list;

		if (process_context && need_resched())
			cond_resched();
		rth = rcu_dereference_raw(rt_hash_table[i].chain);
		if (!rth)
			continue;

		spin_lock_bh(rt_hash_lock_addr(i));

		list = NULL;
		pprev = &rt_hash_table[i].chain;
		rth = rcu_dereference_protected(*pprev,
			lockdep_is_held(rt_hash_lock_addr(i)));

		while (rth) {
			next = rcu_dereference_protected(rth->dst.rt_next,
				lockdep_is_held(rt_hash_lock_addr(i)));

			if (!net ||
			    net_eq(dev_net(rth->dst.dev), net)) {
				rcu_assign_pointer(*pprev, next);
				rcu_assign_pointer(rth->dst.rt_next, list);
				list = rth;
			} else {
				pprev = &rth->dst.rt_next;
			}
			rth = next;
		}

		spin_unlock_bh(rt_hash_lock_addr(i));

		for (; list; list = next) {
			next = rcu_dereference_protected(list->dst.rt_next, 1);
			rt_free(list);
		}
	}
}
