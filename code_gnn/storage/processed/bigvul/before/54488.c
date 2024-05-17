static bool numamigrate_update_ratelimit(pg_data_t *pgdat,
					unsigned long nr_pages)
{
	 
	if (time_after(jiffies, pgdat->numabalancing_migrate_next_window)) {
		spin_lock(&pgdat->numabalancing_migrate_lock);
		pgdat->numabalancing_migrate_nr_pages = 0;
		pgdat->numabalancing_migrate_next_window = jiffies +
			msecs_to_jiffies(migrate_interval_millisecs);
		spin_unlock(&pgdat->numabalancing_migrate_lock);
	}
	if (pgdat->numabalancing_migrate_nr_pages > ratelimit_pages) {
		trace_mm_numa_migrate_ratelimit(current, pgdat->node_id,
								nr_pages);
		return true;
	}

	 
	pgdat->numabalancing_migrate_nr_pages += nr_pages;
	return false;
}
