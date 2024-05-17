static void release_hash_device(struct hash_device_data *device_data)
{
	spin_lock(&device_data->ctx_lock);
	device_data->current_ctx->device = NULL;
	device_data->current_ctx = NULL;
	spin_unlock(&device_data->ctx_lock);

	 
	up(&driver_data.device_allocation);
}
