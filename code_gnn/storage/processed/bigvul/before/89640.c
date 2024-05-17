static int mdio_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	int rc;

	 
	rc = of_device_uevent_modalias(dev, env);
	if (rc != -ENODEV)
		return rc;

	return 0;
}
