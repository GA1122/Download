static int map_create(union bpf_attr *attr)
{
	struct bpf_map *map;
	int err;

	err = CHECK_ATTR(BPF_MAP_CREATE);
	if (err)
		return -EINVAL;

	 
	map = find_and_alloc_map(attr);
	if (IS_ERR(map))
		return PTR_ERR(map);

	atomic_set(&map->refcnt, 1);
	atomic_set(&map->usercnt, 1);

	err = bpf_map_charge_memlock(map);
	if (err)
		goto free_map;

	err = bpf_map_new_fd(map);
	if (err < 0)
		 
		goto free_map;

	return err;

free_map:
	map->ops->map_free(map);
	return err;
}