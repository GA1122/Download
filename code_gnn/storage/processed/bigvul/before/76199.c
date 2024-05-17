static int sort_idmaps(struct uid_gid_map *map)
{
	if (map->nr_extents <= UID_GID_MAP_MAX_BASE_EXTENTS)
		return 0;

	 
	sort(map->forward, map->nr_extents, sizeof(struct uid_gid_extent),
	     cmp_extents_forward, NULL);

	 
	map->reverse = kmemdup(map->forward,
			       map->nr_extents * sizeof(struct uid_gid_extent),
			       GFP_KERNEL);
	if (!map->reverse)
		return -ENOMEM;

	 
	sort(map->reverse, map->nr_extents, sizeof(struct uid_gid_extent),
	     cmp_extents_reverse, NULL);

	return 0;
}
