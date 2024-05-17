int dcb_ieee_delapp(struct net_device *dev, struct dcb_app *del)
{
	struct dcb_app_type *itr;
	struct dcb_app_type event;
	int err = -ENOENT;

	event.ifindex = dev->ifindex;
	memcpy(&event.app, del, sizeof(event.app));
	if (dev->dcbnl_ops->getdcbx)
		event.dcbx = dev->dcbnl_ops->getdcbx(dev);

	spin_lock(&dcb_lock);
	 
	if ((itr = dcb_app_lookup(del, dev->ifindex, del->priority))) {
		list_del(&itr->list);
		kfree(itr);
		err = 0;
	}

	spin_unlock(&dcb_lock);
	if (!err)
		call_dcbevent_notifiers(DCB_APP_EVENT, &event);
	return err;
}