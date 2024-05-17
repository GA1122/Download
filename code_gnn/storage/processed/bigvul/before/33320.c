static int choose_config(struct usb_device *dev)
{
	 

	dev_dbg(&dev->dev, "%s - Number of Interfaces = %d\n",
		__func__, dev->config->desc.bNumInterfaces);
	dev_dbg(&dev->dev, "%s - MAX Power            = %d\n",
		__func__, dev->config->desc.bMaxPower * 2);

	if (dev->config->desc.bNumInterfaces != 1) {
		dev_err(&dev->dev, "%s - bNumInterfaces is not 1, ERROR!\n", __func__);
		return -ENODEV;
	}

	return 0;
}
