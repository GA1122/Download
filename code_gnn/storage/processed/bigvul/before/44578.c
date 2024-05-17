int lxc_assign_network(struct lxc_list *network, pid_t pid)
{
	struct lxc_list *iterator;
	struct lxc_netdev *netdev;
	int am_root = (getuid() == 0);
	int err;

	lxc_list_for_each(iterator, network) {

		netdev = iterator->elem;

		if (netdev->type == LXC_NET_VETH && !am_root) {
			if (unpriv_assign_nic(netdev, pid))
				return -1;
			continue;
		}

		 
		if (!netdev->ifindex)
			continue;

		err = lxc_netdev_move_by_index(netdev->ifindex, pid, NULL);
		if (err) {
			ERROR("failed to move '%s' to the container : %s",
			      netdev->link, strerror(-err));
			return -1;
		}

		DEBUG("move '%s' to '%d'", netdev->name, pid);
	}

	return 0;
}