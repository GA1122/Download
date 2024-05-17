static int do_end_io(struct multipath *m, struct request *clone,
		     int error, struct dm_mpath_io *mpio)
{
	 
	int r = DM_ENDIO_REQUEUE;
	unsigned long flags;

	if (!error && !clone->errors)
		return 0;	 

	if (error == -EOPNOTSUPP || error == -EREMOTEIO || error == -EILSEQ)
		return error;

	if (mpio->pgpath)
		fail_path(mpio->pgpath);

	spin_lock_irqsave(&m->lock, flags);
	if (!m->nr_valid_paths) {
		if (!m->queue_if_no_path) {
			if (!__must_push_back(m))
				r = -EIO;
		} else {
			if (error == -EBADE)
				r = error;
		}
	}
	spin_unlock_irqrestore(&m->lock, flags);

	return r;
}
