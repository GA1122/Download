int dev_close_many(struct list_head *head)
{
	struct net_device *dev, *tmp;
	LIST_HEAD(tmp_list);

	list_for_each_entry_safe(dev, tmp, head, unreg_list)
		if (!(dev->flags & IFF_UP))
			list_move(&dev->unreg_list, &tmp_list);

	__dev_close_many(head);

	 
	list_for_each_entry(dev, head, unreg_list) {
		rtmsg_ifinfo(RTM_NEWLINK, dev, IFF_UP|IFF_RUNNING);
		call_netdevice_notifiers(NETDEV_DOWN, dev);
	}

	 
	list_splice(&tmp_list, head);
	return 0;
}
