int __init_or_module __platform_driver_probe(struct platform_driver *drv,
		int (*probe)(struct platform_device *), struct module *module)
{
	int retval, code;

	if (drv->driver.probe_type == PROBE_PREFER_ASYNCHRONOUS) {
		pr_err("%s: drivers registered with %s can not be probed asynchronously\n",
			 drv->driver.name, __func__);
		return -EINVAL;
	}

	 
	drv->driver.probe_type = PROBE_FORCE_SYNCHRONOUS;

	 
	drv->prevent_deferred_probe = true;

	 
	drv->driver.suppress_bind_attrs = true;

	 
	drv->probe = probe;
	retval = code = __platform_driver_register(drv, module);

	 
	spin_lock(&drv->driver.bus->p->klist_drivers.k_lock);
	drv->probe = NULL;
	if (code == 0 && list_empty(&drv->driver.p->klist_devices.k_list))
		retval = -ENODEV;
	drv->driver.probe = platform_drv_probe_fail;
	spin_unlock(&drv->driver.bus->p->klist_drivers.k_lock);

	if (code != retval)
		platform_driver_unregister(drv);
	return retval;
}
