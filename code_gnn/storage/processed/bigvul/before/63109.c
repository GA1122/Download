static int platform_match(struct device *dev, struct device_driver *drv)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct platform_driver *pdrv = to_platform_driver(drv);

	 
	if (pdev->driver_override)
		return !strcmp(pdev->driver_override, drv->name);

	 
	if (of_driver_match_device(dev, drv))
		return 1;

	 
	if (acpi_driver_match_device(dev, drv))
		return 1;

	 
	if (pdrv->id_table)
		return platform_match_id(pdrv->id_table, pdev) != NULL;

	 
	return (strcmp(pdev->name, drv->name) == 0);
}
