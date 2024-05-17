static unsigned long perf_event_encode(const struct perf_event_map *pmap)
{
	return ((unsigned long) pmap->encoding << 16) | pmap->pic_mask;
}
