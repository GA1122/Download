static void pg_init_done(void *data, int errors)
{
	struct pgpath *pgpath = data;
	struct priority_group *pg = pgpath->pg;
	struct multipath *m = pg->m;
	unsigned long flags;
	unsigned delay_retry = 0;

	 
	switch (errors) {
	case SCSI_DH_OK:
		break;
	case SCSI_DH_NOSYS:
		if (!m->hw_handler_name) {
			errors = 0;
			break;
		}
		DMERR("Could not failover the device: Handler scsi_dh_%s "
		      "Error %d.", m->hw_handler_name, errors);
		 
		fail_path(pgpath);
		break;
	case SCSI_DH_DEV_TEMP_BUSY:
		 
		bypass_pg(m, pg, 1);
		break;
	case SCSI_DH_RETRY:
		 
		delay_retry = 1;
	case SCSI_DH_IMM_RETRY:
	case SCSI_DH_RES_TEMP_UNAVAIL:
		if (pg_init_limit_reached(m, pgpath))
			fail_path(pgpath);
		errors = 0;
		break;
	default:
		 
		fail_path(pgpath);
	}

	spin_lock_irqsave(&m->lock, flags);
	if (errors) {
		if (pgpath == m->current_pgpath) {
			DMERR("Could not failover device. Error %d.", errors);
			m->current_pgpath = NULL;
			m->current_pg = NULL;
		}
	} else if (!m->pg_init_required)
		pg->bypassed = 0;

	if (--m->pg_init_in_progress)
		 
		goto out;

	if (!m->pg_init_required)
		m->queue_io = 0;

	m->pg_init_delay_retry = delay_retry;
	queue_work(kmultipathd, &m->process_queued_ios);

	 
	wake_up(&m->pg_init_wait);

out:
	spin_unlock_irqrestore(&m->lock, flags);
}
