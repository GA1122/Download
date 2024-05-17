static int ux500_hash_remove(struct platform_device *pdev)
{
	struct resource		*res;
	struct hash_device_data *device_data;
	struct device		*dev = &pdev->dev;

	device_data = platform_get_drvdata(pdev);
	if (!device_data) {
		dev_err(dev, "%s: platform_get_drvdata() failed!\n", __func__);
		return -ENOMEM;
	}

	 
	if (down_trylock(&driver_data.device_allocation))
		return -EBUSY;

	 
	spin_lock(&device_data->ctx_lock);
	 
	if (device_data->current_ctx) {
		 
		spin_unlock(&device_data->ctx_lock);
		 
		up(&driver_data.device_allocation);
		return -EBUSY;
	}

	spin_unlock(&device_data->ctx_lock);

	 
	if (klist_node_attached(&device_data->list_node))
		klist_remove(&device_data->list_node);

	 
	if (list_empty(&driver_data.device_list.k_list))
		ahash_algs_unregister_all(device_data);

	if (hash_disable_power(device_data, false))
		dev_err(dev, "%s: hash_disable_power() failed\n",
			__func__);

	clk_unprepare(device_data->clk);
	clk_put(device_data->clk);
	regulator_put(device_data->regulator);

	iounmap(device_data->base);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res)
		release_mem_region(res->start, resource_size(res));

	kfree(device_data);

	return 0;
}
