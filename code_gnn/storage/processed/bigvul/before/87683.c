void hsr_prune_nodes(struct timer_list *t)
{
	struct hsr_priv *hsr = from_timer(hsr, t, prune_timer);
	struct hsr_node *node;
	struct hsr_port *port;
	unsigned long timestamp;
	unsigned long time_a, time_b;

	rcu_read_lock();
	list_for_each_entry_rcu(node, &hsr->node_db, mac_list) {
		 
		time_a = node->time_in[HSR_PT_SLAVE_A];
		time_b = node->time_in[HSR_PT_SLAVE_B];

		 
		if (time_after(jiffies, time_a + MAX_JIFFY_OFFSET/2))
			node->time_in_stale[HSR_PT_SLAVE_A] = true;
		if (time_after(jiffies, time_b + MAX_JIFFY_OFFSET/2))
			node->time_in_stale[HSR_PT_SLAVE_B] = true;

		 
		timestamp = time_a;
		if (node->time_in_stale[HSR_PT_SLAVE_A] ||
		    (!node->time_in_stale[HSR_PT_SLAVE_B] &&
		    time_after(time_b, time_a)))
			timestamp = time_b;

		 
		if (time_is_after_jiffies(timestamp +
					msecs_to_jiffies(1.5*MAX_SLAVE_DIFF))) {
			rcu_read_lock();
			port = get_late_port(hsr, node);
			if (port != NULL)
				hsr_nl_ringerror(hsr, node->MacAddressA, port);
			rcu_read_unlock();
		}

		 
		if (time_is_before_jiffies(timestamp +
					msecs_to_jiffies(HSR_NODE_FORGET_TIME))) {
			hsr_nl_nodedown(hsr, node->MacAddressA);
			list_del_rcu(&node->mac_list);
			 
			kfree_rcu(node, rcu_head);
		}
	}
	rcu_read_unlock();
}