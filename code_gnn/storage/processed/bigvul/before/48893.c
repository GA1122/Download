void netdev_rx_handler_unregister(struct net_device *dev)
{

	ASSERT_RTNL();
	RCU_INIT_POINTER(dev->rx_handler, NULL);
	 
	synchronize_net();
	RCU_INIT_POINTER(dev->rx_handler_data, NULL);
}
