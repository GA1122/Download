static void handle_new_recv_msgs(struct ipmi_smi *intf)
{
	struct ipmi_smi_msg  *smi_msg;
	unsigned long        flags = 0;
	int                  rv;
	int                  run_to_completion = intf->run_to_completion;

	 
	if (!run_to_completion)
		spin_lock_irqsave(&intf->waiting_rcv_msgs_lock, flags);
	while (!list_empty(&intf->waiting_rcv_msgs)) {
		smi_msg = list_entry(intf->waiting_rcv_msgs.next,
				     struct ipmi_smi_msg, link);
		list_del(&smi_msg->link);
		if (!run_to_completion)
			spin_unlock_irqrestore(&intf->waiting_rcv_msgs_lock,
					       flags);
		rv = handle_one_recv_msg(intf, smi_msg);
		if (!run_to_completion)
			spin_lock_irqsave(&intf->waiting_rcv_msgs_lock, flags);
		if (rv > 0) {
			 
			list_add(&smi_msg->link, &intf->waiting_rcv_msgs);
			break;
		} else {
			if (rv == 0)
				 
				ipmi_free_smi_msg(smi_msg);
			 
		}
	}
	if (!run_to_completion)
		spin_unlock_irqrestore(&intf->waiting_rcv_msgs_lock, flags);

	 
	if (atomic_add_unless(&intf->watchdog_pretimeouts_to_deliver, -1, 0)) {
		struct ipmi_user *user;
		int index;

		index = srcu_read_lock(&intf->users_srcu);
		list_for_each_entry_rcu(user, &intf->users, link) {
			if (user->handler->ipmi_watchdog_pretimeout)
				user->handler->ipmi_watchdog_pretimeout(
					user->handler_data);
		}
		srcu_read_unlock(&intf->users_srcu, index);
	}
}
