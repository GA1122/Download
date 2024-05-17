int ipmi_register_for_cmd(struct ipmi_user *user,
			  unsigned char netfn,
			  unsigned char cmd,
			  unsigned int  chans)
{
	struct ipmi_smi *intf = user->intf;
	struct cmd_rcvr *rcvr;
	int rv = 0, index;

	user = acquire_ipmi_user(user, &index);
	if (!user)
		return -ENODEV;

	rcvr = kmalloc(sizeof(*rcvr), GFP_KERNEL);
	if (!rcvr) {
		rv = -ENOMEM;
		goto out_release;
	}
	rcvr->cmd = cmd;
	rcvr->netfn = netfn;
	rcvr->chans = chans;
	rcvr->user = user;

	mutex_lock(&intf->cmd_rcvrs_mutex);
	 
	if (!is_cmd_rcvr_exclusive(intf, netfn, cmd, chans)) {
		rv = -EBUSY;
		goto out_unlock;
	}

	if (atomic_inc_return(&intf->event_waiters) == 1)
		need_waiter(intf);

	list_add_rcu(&rcvr->link, &intf->cmd_rcvrs);

out_unlock:
	mutex_unlock(&intf->cmd_rcvrs_mutex);
	if (rv)
		kfree(rcvr);
out_release:
	release_ipmi_user(user, index);

	return rv;
}