static int multipath_status(struct dm_target *ti, status_type_t type,
			    char *result, unsigned int maxlen)
{
	int sz = 0;
	unsigned long flags;
	struct multipath *m = (struct multipath *) ti->private;
	struct priority_group *pg;
	struct pgpath *p;
	unsigned pg_num;
	char state;

	spin_lock_irqsave(&m->lock, flags);

	 
	if (type == STATUSTYPE_INFO)
		DMEMIT("2 %u %u ", m->queue_size, m->pg_init_count);
	else {
		DMEMIT("%u ", m->queue_if_no_path +
			      (m->pg_init_retries > 0) * 2 +
			      (m->pg_init_delay_msecs != DM_PG_INIT_DELAY_DEFAULT) * 2);
		if (m->queue_if_no_path)
			DMEMIT("queue_if_no_path ");
		if (m->pg_init_retries)
			DMEMIT("pg_init_retries %u ", m->pg_init_retries);
		if (m->pg_init_delay_msecs != DM_PG_INIT_DELAY_DEFAULT)
			DMEMIT("pg_init_delay_msecs %u ", m->pg_init_delay_msecs);
	}

	if (!m->hw_handler_name || type == STATUSTYPE_INFO)
		DMEMIT("0 ");
	else
		DMEMIT("1 %s ", m->hw_handler_name);

	DMEMIT("%u ", m->nr_priority_groups);

	if (m->next_pg)
		pg_num = m->next_pg->pg_num;
	else if (m->current_pg)
		pg_num = m->current_pg->pg_num;
	else
		pg_num = (m->nr_priority_groups ? 1 : 0);

	DMEMIT("%u ", pg_num);

	switch (type) {
	case STATUSTYPE_INFO:
		list_for_each_entry(pg, &m->priority_groups, list) {
			if (pg->bypassed)
				state = 'D';	 
			else if (pg == m->current_pg)
				state = 'A';	 
			else
				state = 'E';	 

			DMEMIT("%c ", state);

			if (pg->ps.type->status)
				sz += pg->ps.type->status(&pg->ps, NULL, type,
							  result + sz,
							  maxlen - sz);
			else
				DMEMIT("0 ");

			DMEMIT("%u %u ", pg->nr_pgpaths,
			       pg->ps.type->info_args);

			list_for_each_entry(p, &pg->pgpaths, list) {
				DMEMIT("%s %s %u ", p->path.dev->name,
				       p->is_active ? "A" : "F",
				       p->fail_count);
				if (pg->ps.type->status)
					sz += pg->ps.type->status(&pg->ps,
					      &p->path, type, result + sz,
					      maxlen - sz);
			}
		}
		break;

	case STATUSTYPE_TABLE:
		list_for_each_entry(pg, &m->priority_groups, list) {
			DMEMIT("%s ", pg->ps.type->name);

			if (pg->ps.type->status)
				sz += pg->ps.type->status(&pg->ps, NULL, type,
							  result + sz,
							  maxlen - sz);
			else
				DMEMIT("0 ");

			DMEMIT("%u %u ", pg->nr_pgpaths,
			       pg->ps.type->table_args);

			list_for_each_entry(p, &pg->pgpaths, list) {
				DMEMIT("%s ", p->path.dev->name);
				if (pg->ps.type->status)
					sz += pg->ps.type->status(&pg->ps,
					      &p->path, type, result + sz,
					      maxlen - sz);
			}
		}
		break;
	}

	spin_unlock_irqrestore(&m->lock, flags);

	return 0;
}