static void __switch_pg(struct multipath *m, struct pgpath *pgpath)
{
	m->current_pg = pgpath->pg;

	 
	if (m->hw_handler_name) {
		m->pg_init_required = 1;
		m->queue_io = 1;
	} else {
		m->pg_init_required = 0;
		m->queue_io = 0;
	}

	m->pg_init_count = 0;
}