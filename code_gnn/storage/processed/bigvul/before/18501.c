static pg_data_t __ref *hotadd_new_pgdat(int nid, u64 start)
{
	struct pglist_data *pgdat;
	unsigned long zones_size[MAX_NR_ZONES] = {0};
	unsigned long zholes_size[MAX_NR_ZONES] = {0};
	unsigned long start_pfn = start >> PAGE_SHIFT;

	pgdat = arch_alloc_nodedata(nid);
	if (!pgdat)
		return NULL;

	arch_refresh_nodedata(nid, pgdat);

	 

	 
	free_area_init_node(nid, zones_size, start_pfn, zholes_size);

	 
	mutex_lock(&zonelists_mutex);
	build_all_zonelists(pgdat, NULL);
	mutex_unlock(&zonelists_mutex);

	return pgdat;
}
