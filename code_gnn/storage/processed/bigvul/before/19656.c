static void kvp_pool_enumerate(int pool, int index, __u8 *key, int key_size,
				__u8 *value, int value_size)
{
	struct kvp_record *record;

	 
	kvp_update_mem_state(pool);
	record = kvp_file_info[pool].records;

	if (index >= kvp_file_info[pool].num_records) {
		 
		strcpy(value, "");
		return;
	}

	memcpy(key, record[index].key, key_size);
	memcpy(value, record[index].value, value_size);
}
