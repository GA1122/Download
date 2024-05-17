int init_dummy_netdev(struct net_device *dev)
{
	 
	memset(dev, 0, sizeof(struct net_device));

	 
	dev->reg_state = NETREG_DUMMY;

	 
	INIT_LIST_HEAD(&dev->napi_list);

	 
	set_bit(__LINK_STATE_PRESENT, &dev->state);
	set_bit(__LINK_STATE_START, &dev->state);

	 

	return 0;
}
