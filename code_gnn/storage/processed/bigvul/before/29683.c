static void persistent_usage(struct dm_exception_store *store,
			     sector_t *total_sectors,
			     sector_t *sectors_allocated,
			     sector_t *metadata_sectors)
{
	struct pstore *ps = get_info(store);

	*sectors_allocated = ps->next_free * store->chunk_size;
	*total_sectors = get_dev_size(dm_snap_cow(store->snap)->bdev);

	 
	*metadata_sectors = (ps->current_area + 1 + NUM_SNAPSHOT_HDR_CHUNKS) *
			    store->chunk_size;
}