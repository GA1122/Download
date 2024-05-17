static int set_recommended_min_free_kbytes(void)
{
	struct zone *zone;
	int nr_zones = 0;
	unsigned long recommended_min;
	extern int min_free_kbytes;

	if (!test_bit(TRANSPARENT_HUGEPAGE_FLAG,
		      &transparent_hugepage_flags) &&
	    !test_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
		      &transparent_hugepage_flags))
		return 0;

	for_each_populated_zone(zone)
		nr_zones++;

	 
	recommended_min = pageblock_nr_pages * nr_zones * 2;

	 
	recommended_min += pageblock_nr_pages * nr_zones *
			   MIGRATE_PCPTYPES * MIGRATE_PCPTYPES;

	 
	recommended_min = min(recommended_min,
			      (unsigned long) nr_free_buffer_pages() / 20);
	recommended_min <<= (PAGE_SHIFT-10);

	if (recommended_min > min_free_kbytes)
		min_free_kbytes = recommended_min;
	setup_per_zone_wmarks();
	return 0;
}
