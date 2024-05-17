static void resv_map_release(struct kref *ref)
{
	struct resv_map *resv_map = container_of(ref, struct resv_map, refs);

	 
	region_truncate(&resv_map->regions, 0);
	kfree(resv_map);
}
