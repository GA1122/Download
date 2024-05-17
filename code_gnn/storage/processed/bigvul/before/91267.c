int ipmi_create_user(unsigned int          if_num,
		     const struct ipmi_user_hndl *handler,
		     void                  *handler_data,
		     struct ipmi_user      **user)
{
	unsigned long flags;
	struct ipmi_user *new_user;
	int           rv = 0, index;
	struct ipmi_smi *intf;

	 

	if (handler == NULL)
		return -EINVAL;

	 
	if (!initialized) {
		rv = ipmi_init_msghandler();
		if (rv)
			return rv;

		 
		if (!initialized)
			return -ENODEV;
	}

	new_user = kmalloc(sizeof(*new_user), GFP_KERNEL);
	if (!new_user)
		return -ENOMEM;

	index = srcu_read_lock(&ipmi_interfaces_srcu);
	list_for_each_entry_rcu(intf, &ipmi_interfaces, link) {
		if (intf->intf_num == if_num)
			goto found;
	}
	 
	rv = -EINVAL;
	goto out_kfree;

 found:
	rv = init_srcu_struct(&new_user->release_barrier);
	if (rv)
		goto out_kfree;

	 
	kref_get(&intf->refcount);

	kref_init(&new_user->refcount);
	new_user->handler = handler;
	new_user->handler_data = handler_data;
	new_user->intf = intf;
	new_user->gets_events = false;

	rcu_assign_pointer(new_user->self, new_user);
	spin_lock_irqsave(&intf->seq_lock, flags);
	list_add_rcu(&new_user->link, &intf->users);
	spin_unlock_irqrestore(&intf->seq_lock, flags);
	if (handler->ipmi_watchdog_pretimeout) {
		 
		if (atomic_inc_return(&intf->event_waiters) == 1)
			need_waiter(intf);
	}
	srcu_read_unlock(&ipmi_interfaces_srcu, index);
	*user = new_user;
	return 0;

out_kfree:
	srcu_read_unlock(&ipmi_interfaces_srcu, index);
	kfree(new_user);
	return rv;
}
