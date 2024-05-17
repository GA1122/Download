static void _ipmi_destroy_user(struct ipmi_user *user)
{
	struct ipmi_smi  *intf = user->intf;
	int              i;
	unsigned long    flags;
	struct cmd_rcvr  *rcvr;
	struct cmd_rcvr  *rcvrs = NULL;

	if (!acquire_ipmi_user(user, &i)) {
		 
		synchronize_srcu(&user->release_barrier);
		return;
	}

	rcu_assign_pointer(user->self, NULL);
	release_ipmi_user(user, i);

	synchronize_srcu(&user->release_barrier);

	if (user->handler->shutdown)
		user->handler->shutdown(user->handler_data);

	if (user->handler->ipmi_watchdog_pretimeout)
		atomic_dec(&intf->event_waiters);

	if (user->gets_events)
		atomic_dec(&intf->event_waiters);

	 
	spin_lock_irqsave(&intf->seq_lock, flags);
	list_del_rcu(&user->link);

	for (i = 0; i < IPMI_IPMB_NUM_SEQ; i++) {
		if (intf->seq_table[i].inuse
		    && (intf->seq_table[i].recv_msg->user == user)) {
			intf->seq_table[i].inuse = 0;
			ipmi_free_recv_msg(intf->seq_table[i].recv_msg);
		}
	}
	spin_unlock_irqrestore(&intf->seq_lock, flags);

	 
	mutex_lock(&intf->cmd_rcvrs_mutex);
	list_for_each_entry_rcu(rcvr, &intf->cmd_rcvrs, link) {
		if (rcvr->user == user) {
			list_del_rcu(&rcvr->link);
			rcvr->next = rcvrs;
			rcvrs = rcvr;
		}
	}
	mutex_unlock(&intf->cmd_rcvrs_mutex);
	synchronize_rcu();
	while (rcvrs) {
		rcvr = rcvrs;
		rcvrs = rcvr->next;
		kfree(rcvr);
	}

	kref_put(&intf->refcount, intf_free);
}