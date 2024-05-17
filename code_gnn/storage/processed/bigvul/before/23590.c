static int map_io(struct multipath *m, struct request *clone,
		  struct dm_mpath_io *mpio, unsigned was_queued)
{
	int r = DM_MAPIO_REMAPPED;
	size_t nr_bytes = blk_rq_bytes(clone);
	unsigned long flags;
	struct pgpath *pgpath;
	struct block_device *bdev;

	spin_lock_irqsave(&m->lock, flags);

	 
	if (!m->current_pgpath ||
	    (!m->queue_io && (m->repeat_count && --m->repeat_count == 0)))
		__choose_pgpath(m, nr_bytes);

	pgpath = m->current_pgpath;

	if (was_queued)
		m->queue_size--;

	if ((pgpath && m->queue_io) ||
	    (!pgpath && m->queue_if_no_path)) {
		 
		list_add_tail(&clone->queuelist, &m->queued_ios);
		m->queue_size++;
		if ((m->pg_init_required && !m->pg_init_in_progress) ||
		    !m->queue_io)
			queue_work(kmultipathd, &m->process_queued_ios);
		pgpath = NULL;
		r = DM_MAPIO_SUBMITTED;
	} else if (pgpath) {
		bdev = pgpath->path.dev->bdev;
		clone->q = bdev_get_queue(bdev);
		clone->rq_disk = bdev->bd_disk;
	} else if (__must_push_back(m))
		r = DM_MAPIO_REQUEUE;
	else
		r = -EIO;	 

	mpio->pgpath = pgpath;
	mpio->nr_bytes = nr_bytes;

	if (r == DM_MAPIO_REMAPPED && pgpath->pg->ps.type->start_io)
		pgpath->pg->ps.type->start_io(&pgpath->pg->ps, &pgpath->path,
					      nr_bytes);

	spin_unlock_irqrestore(&m->lock, flags);

	return r;
}