static int gs_can_close(struct net_device *netdev)
{
	int rc;
	struct gs_can *dev = netdev_priv(netdev);
	struct gs_usb *parent = dev->parent;

	netif_stop_queue(netdev);

	 
	if (atomic_dec_and_test(&parent->active_channels))
		usb_kill_anchored_urbs(&parent->rx_submitted);

	 
	usb_kill_anchored_urbs(&dev->tx_submitted);
	atomic_set(&dev->active_tx_urbs, 0);

	 
	rc = gs_cmd_reset(parent, dev);
	if (rc < 0)
		netdev_warn(netdev, "Couldn't shutdown device (err=%d)", rc);

	 
	for (rc = 0; rc < GS_MAX_TX_URBS; rc++) {
		dev->tx_context[rc].dev = dev;
		dev->tx_context[rc].echo_id = GS_MAX_TX_URBS;
	}

	 
	close_candev(netdev);

	return 0;
}