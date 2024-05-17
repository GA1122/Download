int dcb_setapp(struct net_device *dev, struct dcb_app *new)
{
	struct dcb_app_type *itr;
	struct dcb_app_type event;
	int err = 0;

	event.ifindex = dev->ifindex;
	memcpy(&event.app, new, sizeof(event.app));
	if (dev->dcbnl_ops->getdcbx)
		event.dcbx = dev->dcbnl_ops->getdcbx(dev);

	spin_lock(&dcb_lock);
	 
	if ((itr = dcb_app_lookup(new, dev->ifindex, 0))) {
		if (new->priority)
			itr->app.priority = new->priority;
		else {
			list_del(&itr->list);
			kfree(itr);
		}
		goto out;
	}
	 
	if (new->priority)
		err = dcb_app_add(new, dev->ifindex);
out:
	spin_unlock(&dcb_lock);
	if (!err)
		call_dcbevent_notifiers(DCB_APP_EVENT, &event);
	return err;
}