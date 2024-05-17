static void __choose_pgpath(struct multipath *m, size_t nr_bytes)
{
	struct priority_group *pg;
	unsigned bypassed = 1;

	if (!m->nr_valid_paths)
		goto failed;

	 
	if (m->next_pg) {
		pg = m->next_pg;
		m->next_pg = NULL;
		if (!__choose_path_in_pg(m, pg, nr_bytes))
			return;
	}

	 
	if (m->current_pg && !__choose_path_in_pg(m, m->current_pg, nr_bytes))
		return;

	 
	do {
		list_for_each_entry(pg, &m->priority_groups, list) {
			if (pg->bypassed == bypassed)
				continue;
			if (!__choose_path_in_pg(m, pg, nr_bytes))
				return;
		}
	} while (bypassed--);

failed:
	m->current_pgpath = NULL;
	m->current_pg = NULL;
}
