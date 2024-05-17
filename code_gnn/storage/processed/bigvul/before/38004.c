char *netdev_drivername(const struct net_device *dev, char *buffer, int len)
{
	const struct device_driver *driver;
	const struct device *parent;

	if (len <= 0 || !buffer)
		return buffer;
	buffer[0] = 0;

	parent = dev->dev.parent;

	if (!parent)
		return buffer;

	driver = parent->driver;
	if (driver && driver->name)
		strlcpy(buffer, driver->name, len);
 	return buffer;
 }