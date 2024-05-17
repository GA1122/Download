void lxc_delete_network(struct lxc_handler *handler)
{
	struct lxc_list *network = &handler->conf->network;
	struct lxc_list *iterator;
	struct lxc_netdev *netdev;

	lxc_list_for_each(iterator, network) {
		netdev = iterator->elem;

		if (netdev->ifindex != 0 && netdev->type == LXC_NET_PHYS) {
			if (lxc_netdev_rename_by_index(netdev->ifindex, netdev->link))
				WARN("failed to rename to the initial name the " \
				     "netdev '%s'", netdev->link);
			continue;
		}

		if (netdev_deconf[netdev->type](handler, netdev)) {
			WARN("failed to destroy netdev");
		}

		 
		if (netdev->ifindex != 0 &&
		    lxc_netdev_delete_by_index(netdev->ifindex))
			WARN("failed to remove interface '%s'", netdev->name);
	}
}