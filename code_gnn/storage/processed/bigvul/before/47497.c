static int cryp_get_device_data(struct cryp_ctx *ctx,
				struct cryp_device_data **device_data)
{
	int ret;
	struct klist_iter device_iterator;
	struct klist_node *device_node;
	struct cryp_device_data *local_device_data = NULL;
	pr_debug(DEV_DBG_NAME " [%s]", __func__);

	 
	ret = down_interruptible(&driver_data.device_allocation);
	if (ret)
		return ret;   

	 
	klist_iter_init(&driver_data.device_list, &device_iterator);

	device_node = klist_next(&device_iterator);
	while (device_node) {
		local_device_data = container_of(device_node,
					   struct cryp_device_data, list_node);
		spin_lock(&local_device_data->ctx_lock);
		 
		if (local_device_data->current_ctx) {
			device_node = klist_next(&device_iterator);
		} else {
			local_device_data->current_ctx = ctx;
			ctx->device = local_device_data;
			spin_unlock(&local_device_data->ctx_lock);
			break;
		}
		spin_unlock(&local_device_data->ctx_lock);
	}
	klist_iter_exit(&device_iterator);

	if (!device_node) {
		 
		return -EBUSY;
	}

	*device_data = local_device_data;

	return 0;
}
