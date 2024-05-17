void hash_begin(struct hash_device_data *device_data, struct hash_ctx *ctx)
{
	 
	 

	while (readl(&device_data->base->str) & HASH_STR_DCAL_MASK)
		cpu_relax();

	 
	HASH_INITIALIZE;

	 
	HASH_CLEAR_BITS(&device_data->base->str, HASH_STR_NBLW_MASK);
}
