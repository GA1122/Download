static void hostap_set_multicast_list(struct net_device *dev)
{
#if 0
	 
	struct hostap_interface *iface;
	local_info_t *local;

	iface = netdev_priv(dev);
	local = iface->local;
	if ((dev->flags & IFF_ALLMULTI) || (dev->flags & IFF_PROMISC)) {
		local->is_promisc = 1;
	} else {
		local->is_promisc = 0;
	}

	schedule_work(&local->set_multicast_list_queue);
#endif
}
