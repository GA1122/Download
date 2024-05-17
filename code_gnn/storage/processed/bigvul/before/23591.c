static int multipath_busy(struct dm_target *ti)
{
	int busy = 0, has_active = 0;
	struct multipath *m = ti->private;
	struct priority_group *pg;
	struct pgpath *pgpath;
	unsigned long flags;

	spin_lock_irqsave(&m->lock, flags);

	 
	if (unlikely(!m->current_pgpath && m->next_pg))
		pg = m->next_pg;
	else if (likely(m->current_pg))
		pg = m->current_pg;
	else
		 
		goto out;

	 
	busy = 1;
	list_for_each_entry(pgpath, &pg->pgpaths, list)
		if (pgpath->is_active) {
			has_active = 1;

			if (!__pgpath_busy(pgpath)) {
				busy = 0;
				break;
			}
		}

	if (!has_active)
		 
		busy = 0;

out:
	spin_unlock_irqrestore(&m->lock, flags);

	return busy;
}
