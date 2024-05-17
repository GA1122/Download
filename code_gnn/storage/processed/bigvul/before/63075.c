void __init early_platform_add_devices(struct platform_device **devs, int num)
{
	struct device *dev;
	int i;

	 
	for (i = 0; i < num; i++) {
		dev = &devs[i]->dev;

		if (!dev->devres_head.next) {
			pm_runtime_early_init(dev);
			INIT_LIST_HEAD(&dev->devres_head);
			list_add_tail(&dev->devres_head,
				      &early_platform_device_list);
		}
	}
}
