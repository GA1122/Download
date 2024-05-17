static int handle_read_event_rsp(struct ipmi_smi *intf,
				 struct ipmi_smi_msg *msg)
{
	struct ipmi_recv_msg *recv_msg, *recv_msg2;
	struct list_head     msgs;
	struct ipmi_user     *user;
	int rv = 0, deliver_count = 0, index;
	unsigned long        flags;

	if (msg->rsp_size < 19) {
		 
		ipmi_inc_stat(intf, invalid_events);
		return 0;
	}

	if (msg->rsp[2] != 0) {
		 
		return 0;
	}

	INIT_LIST_HEAD(&msgs);

	spin_lock_irqsave(&intf->events_lock, flags);

	ipmi_inc_stat(intf, events);

	 
	index = srcu_read_lock(&intf->users_srcu);
	list_for_each_entry_rcu(user, &intf->users, link) {
		if (!user->gets_events)
			continue;

		recv_msg = ipmi_alloc_recv_msg();
		if (!recv_msg) {
			rcu_read_unlock();
			list_for_each_entry_safe(recv_msg, recv_msg2, &msgs,
						 link) {
				list_del(&recv_msg->link);
				ipmi_free_recv_msg(recv_msg);
			}
			 
			rv = 1;
			goto out;
		}

		deliver_count++;

		copy_event_into_recv_msg(recv_msg, msg);
		recv_msg->user = user;
		kref_get(&user->refcount);
		list_add_tail(&recv_msg->link, &msgs);
	}
	srcu_read_unlock(&intf->users_srcu, index);

	if (deliver_count) {
		 
		list_for_each_entry_safe(recv_msg, recv_msg2, &msgs, link) {
			list_del(&recv_msg->link);
			deliver_local_response(intf, recv_msg);
		}
	} else if (intf->waiting_events_count < MAX_EVENTS_IN_QUEUE) {
		 
		recv_msg = ipmi_alloc_recv_msg();
		if (!recv_msg) {
			 
			rv = 1;
			goto out;
		}

		copy_event_into_recv_msg(recv_msg, msg);
		list_add_tail(&recv_msg->link, &intf->waiting_events);
		intf->waiting_events_count++;
	} else if (!intf->event_msg_printed) {
		 
		dev_warn(intf->si_dev,
			 "Event queue full, discarding incoming events\n");
		intf->event_msg_printed = 1;
	}

 out:
	spin_unlock_irqrestore(&intf->events_lock, flags);

	return rv;
}
