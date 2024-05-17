static int multipath_ctr(struct dm_target *ti, unsigned int argc,
			 char **argv)
{
	 
	static struct dm_arg _args[] = {
		{0, 1024, "invalid number of priority groups"},
		{0, 1024, "invalid initial priority group number"},
	};

	int r;
	struct multipath *m;
	struct dm_arg_set as;
	unsigned pg_count = 0;
	unsigned next_pg_num;

	as.argc = argc;
	as.argv = argv;

	m = alloc_multipath(ti);
	if (!m) {
		ti->error = "can't allocate multipath";
		return -EINVAL;
	}

	r = parse_features(&as, m);
	if (r)
		goto bad;

	r = parse_hw_handler(&as, m);
	if (r)
		goto bad;

	r = dm_read_arg(_args, &as, &m->nr_priority_groups, &ti->error);
	if (r)
		goto bad;

	r = dm_read_arg(_args + 1, &as, &next_pg_num, &ti->error);
	if (r)
		goto bad;

	if ((!m->nr_priority_groups && next_pg_num) ||
	    (m->nr_priority_groups && !next_pg_num)) {
		ti->error = "invalid initial priority group";
		r = -EINVAL;
		goto bad;
	}

	 
	while (as.argc) {
		struct priority_group *pg;

		pg = parse_priority_group(&as, m);
		if (IS_ERR(pg)) {
			r = PTR_ERR(pg);
			goto bad;
		}

		m->nr_valid_paths += pg->nr_pgpaths;
		list_add_tail(&pg->list, &m->priority_groups);
		pg_count++;
		pg->pg_num = pg_count;
		if (!--next_pg_num)
			m->next_pg = pg;
	}

	if (pg_count != m->nr_priority_groups) {
		ti->error = "priority group count mismatch";
		r = -EINVAL;
		goto bad;
	}

	ti->num_flush_requests = 1;
	ti->num_discard_requests = 1;

	return 0;

 bad:
	free_multipath(m);
	return r;
}