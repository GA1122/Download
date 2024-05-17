int __ref add_memory(int nid, u64 start, u64 size)
{
	pg_data_t *pgdat = NULL;
	int new_pgdat = 0;
	struct resource *res;
	int ret;

	lock_memory_hotplug();

	res = register_memory_resource(start, size);
	ret = -EEXIST;
	if (!res)
		goto out;

	if (!node_online(nid)) {
		pgdat = hotadd_new_pgdat(nid, start);
		ret = -ENOMEM;
		if (!pgdat)
			goto error;
		new_pgdat = 1;
	}

	 
	ret = arch_add_memory(nid, start, size);

	if (ret < 0)
		goto error;

	 
	node_set_online(nid);

	if (new_pgdat) {
		ret = register_one_node(nid);
		 
		BUG_ON(ret);
	}

	 
	firmware_map_add_hotplug(start, start + size, "System RAM");

	goto out;

error:
	 
	if (new_pgdat)
		rollback_node_hotadd(nid, pgdat);
	if (res)
		release_memory_resource(res);

out:
	unlock_memory_hotplug();
	return ret;
}
