static int fwnet_probe(struct fw_unit *unit,
		       const struct ieee1394_device_id *id)
{
	struct fw_device *device = fw_parent_device(unit);
	struct fw_card *card = device->card;
	struct net_device *net;
	bool allocated_netdev = false;
	struct fwnet_device *dev;
	unsigned max_mtu;
	int ret;
	union fwnet_hwaddr *ha;

	mutex_lock(&fwnet_device_mutex);

	dev = fwnet_dev_find(card);
	if (dev) {
		net = dev->netdev;
		goto have_dev;
	}

	net = alloc_netdev(sizeof(*dev), "firewire%d", NET_NAME_UNKNOWN,
			   fwnet_init_dev);
	if (net == NULL) {
		mutex_unlock(&fwnet_device_mutex);
		return -ENOMEM;
	}

	allocated_netdev = true;
	SET_NETDEV_DEV(net, card->device);
	dev = netdev_priv(net);

	spin_lock_init(&dev->lock);
	dev->broadcast_state = FWNET_BROADCAST_ERROR;
	dev->broadcast_rcv_context = NULL;
	dev->broadcast_xmt_max_payload = 0;
	dev->broadcast_xmt_datagramlabel = 0;
	dev->local_fifo = FWNET_NO_FIFO_ADDR;
	dev->queued_datagrams = 0;
	INIT_LIST_HEAD(&dev->peer_list);
	dev->card = card;
	dev->netdev = net;

	ret = fwnet_fifo_start(dev);
	if (ret < 0)
		goto out;
	dev->local_fifo = dev->handler.offset;

	 
	max_mtu = (1 << (card->max_receive + 1))
		  - sizeof(struct rfc2734_header) - IEEE1394_GASP_HDR_SIZE;
	net->mtu = min(1500U, max_mtu);

	 
	ha = (union fwnet_hwaddr *)net->dev_addr;
	put_unaligned_be64(card->guid, &ha->uc.uniq_id);
	ha->uc.max_rec = dev->card->max_receive;
	ha->uc.sspd = dev->card->link_speed;
	put_unaligned_be16(dev->local_fifo >> 32, &ha->uc.fifo_hi);
	put_unaligned_be32(dev->local_fifo & 0xffffffff, &ha->uc.fifo_lo);

	memset(net->broadcast, -1, net->addr_len);

	ret = register_netdev(net);
	if (ret)
		goto out;

	list_add_tail(&dev->dev_link, &fwnet_device_list);
	dev_notice(&net->dev, "IP over IEEE 1394 on card %s\n",
		   dev_name(card->device));
 have_dev:
	ret = fwnet_add_peer(dev, unit, device);
	if (ret && allocated_netdev) {
		unregister_netdev(net);
		list_del(&dev->dev_link);
 out:
		fwnet_fifo_stop(dev);
		free_netdev(net);
	}

	mutex_unlock(&fwnet_device_mutex);

	return ret;
}