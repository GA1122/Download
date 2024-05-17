static struct blk_desc *get_dev_hwpart(const char *ifname, int dev, int hwpart)
{
	struct blk_desc *dev_desc;
	int ret;

	dev_desc = blk_get_devnum_by_typename(ifname, dev);
	if (!dev_desc) {
		debug("%s: No device for iface '%s', dev %d\n", __func__,
		      ifname, dev);
		return NULL;
	}
	ret = blk_dselect_hwpart(dev_desc, hwpart);
	if (ret) {
		debug("%s: Failed to select h/w partition: err-%d\n", __func__,
		      ret);
		return NULL;
	}

	return dev_desc;
}
