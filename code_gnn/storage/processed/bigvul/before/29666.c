static chunk_t area_location(struct pstore *ps, chunk_t area)
{
 	return NUM_SNAPSHOT_HDR_CHUNKS + ((ps->exceptions_per_area + 1) * area);
 }