static int instantiate_veth(struct lxc_handler *handler, struct lxc_netdev *netdev)
{
	char veth1buf[IFNAMSIZ], *veth1;
	char veth2buf[IFNAMSIZ], *veth2;
	int err, mtu = 0;

	if (netdev->priv.veth_attr.pair) {
		veth1 = netdev->priv.veth_attr.pair;
		if (handler->conf->reboot)
			lxc_netdev_delete_by_name(veth1);
	} else {
		err = snprintf(veth1buf, sizeof(veth1buf), "vethXXXXXX");
		if (err >= sizeof(veth1buf)) {  
			ERROR("veth1 name too long");
			return -1;
		}
		veth1 = lxc_mkifname(veth1buf);
		if (!veth1) {
			ERROR("failed to allocate a temporary name");
			return -1;
		}
		 
		memcpy(netdev->priv.veth_attr.veth1, veth1, IFNAMSIZ);
	}

	snprintf(veth2buf, sizeof(veth2buf), "vethXXXXXX");
	veth2 = lxc_mkifname(veth2buf);
	if (!veth2) {
		ERROR("failed to allocate a temporary name");
		goto out_delete;
	}

	err = lxc_veth_create(veth1, veth2);
	if (err) {
		ERROR("failed to create veth pair (%s and %s): %s", veth1, veth2,
		      strerror(-err));
		goto out_delete;
	}

	 
	err = setup_private_host_hw_addr(veth1);
	if (err) {
		ERROR("failed to change mac address of host interface '%s': %s",
			veth1, strerror(-err));
		goto out_delete;
	}

	netdev->ifindex = if_nametoindex(veth2);
	if (!netdev->ifindex) {
		ERROR("failed to retrieve the index for %s", veth2);
		goto out_delete;
	}

	if (netdev->mtu) {
		mtu = atoi(netdev->mtu);
	} else if (netdev->link) {
		mtu = netdev_get_mtu(netdev->ifindex);
	}

	if (mtu) {
		err = lxc_netdev_set_mtu(veth1, mtu);
		if (!err)
			err = lxc_netdev_set_mtu(veth2, mtu);
		if (err) {
			ERROR("failed to set mtu '%i' for veth pair (%s and %s): %s",
			      mtu, veth1, veth2, strerror(-err));
			goto out_delete;
		}
	}

	if (netdev->link) {
		err = lxc_bridge_attach(netdev->link, veth1);
		if (err) {
			ERROR("failed to attach '%s' to the bridge '%s': %s",
				      veth1, netdev->link, strerror(-err));
			goto out_delete;
		}
	}

	err = lxc_netdev_up(veth1);
	if (err) {
		ERROR("failed to set %s up : %s", veth1, strerror(-err));
		goto out_delete;
	}

	if (netdev->upscript) {
		err = run_script(handler->name, "net", netdev->upscript, "up",
				 "veth", veth1, (char*) NULL);
		if (err)
			goto out_delete;
	}

	DEBUG("instantiated veth '%s/%s', index is '%d'",
	      veth1, veth2, netdev->ifindex);

	return 0;

out_delete:
	lxc_netdev_delete_by_name(veth1);
	if (!netdev->priv.veth_attr.pair)
		free(veth1);
	free(veth2);
	return -1;
}