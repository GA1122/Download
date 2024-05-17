static inline void atalk_dev_down(struct net_device *dev)
{
	atrtr_device_down(dev);	 
	aarp_device_down(dev);	 
	atif_drop_device(dev);	 
}
