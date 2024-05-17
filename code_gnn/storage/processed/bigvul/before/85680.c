static int hns_nic_change_mtu(struct net_device *ndev, int new_mtu)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	bool if_running = netif_running(ndev);
	int ret;

	 
	if (new_mtu < 68)
		return -EINVAL;

	 
	if (new_mtu == ndev->mtu)
		return 0;

	if (!h->dev->ops->set_mtu)
		return -ENOTSUPP;

	if (if_running) {
		(void)hns_nic_net_stop(ndev);
		msleep(100);
	}

	if (priv->enet_ver != AE_VERSION_1 &&
	    ndev->mtu <= BD_SIZE_2048_MAX_MTU &&
	    new_mtu > BD_SIZE_2048_MAX_MTU) {
		 
		hnae_reinit_all_ring_desc(h);

		 
		ret = hns_nic_clear_all_rx_fetch(ndev);

		 
		hnae_reinit_all_ring_page_off(h);

		if (ret) {
			netdev_err(ndev, "clear the fetched desc fail\n");
			goto out;
		}
	}

	ret = h->dev->ops->set_mtu(h, new_mtu);
	if (ret) {
		netdev_err(ndev, "set mtu fail, return value %d\n",
			   ret);
		goto out;
	}

	 
	ndev->mtu = new_mtu;

out:
	if (if_running) {
		if (hns_nic_net_open(ndev)) {
			netdev_err(ndev, "hns net open fail\n");
			ret = -EINVAL;
		}
	}

	return ret;
}
