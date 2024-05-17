static void __pg_init_all_paths(struct multipath *m)
{
	struct pgpath *pgpath;
	unsigned long pg_init_delay = 0;

	m->pg_init_count++;
	m->pg_init_required = 0;
	if (m->pg_init_delay_retry)
		pg_init_delay = msecs_to_jiffies(m->pg_init_delay_msecs != DM_PG_INIT_DELAY_DEFAULT ?
						 m->pg_init_delay_msecs : DM_PG_INIT_DELAY_MSECS);
	list_for_each_entry(pgpath, &m->current_pg->pgpaths, list) {
		 
		if (!pgpath->is_active)
			continue;
		if (queue_delayed_work(kmpath_handlerd, &pgpath->activate_path,
				       pg_init_delay))
			m->pg_init_in_progress++;
	}
}