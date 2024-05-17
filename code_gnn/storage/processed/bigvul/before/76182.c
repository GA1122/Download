static int insert_extent(struct uid_gid_map *map, struct uid_gid_extent *extent)
{
	struct uid_gid_extent *dest;

	if (map->nr_extents == UID_GID_MAP_MAX_BASE_EXTENTS) {
		struct uid_gid_extent *forward;

		 
		forward = kmalloc_array(UID_GID_MAP_MAX_EXTENTS,
					sizeof(struct uid_gid_extent),
					GFP_KERNEL);
		if (!forward)
			return -ENOMEM;

		 
		memcpy(forward, map->extent,
		       map->nr_extents * sizeof(map->extent[0]));

		map->forward = forward;
		map->reverse = NULL;
	}

	if (map->nr_extents < UID_GID_MAP_MAX_BASE_EXTENTS)
		dest = &map->extent[map->nr_extents];
	else
		dest = &map->forward[map->nr_extents];

	*dest = *extent;
	map->nr_extents++;
	return 0;
}
