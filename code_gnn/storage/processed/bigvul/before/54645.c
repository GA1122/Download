int snd_seq_enqueue_event(struct snd_seq_event_cell *cell, int atomic, int hop)
{
	int dest, err;
	struct snd_seq_queue *q;

	if (snd_BUG_ON(!cell))
		return -EINVAL;
	dest = cell->event.queue;	 
	q = queueptr(dest);
	if (q == NULL)
		return -EINVAL;
	 
	if ((cell->event.flags & SNDRV_SEQ_TIME_MODE_MASK) == SNDRV_SEQ_TIME_MODE_REL) {
		switch (cell->event.flags & SNDRV_SEQ_TIME_STAMP_MASK) {
		case SNDRV_SEQ_TIME_STAMP_TICK:
			cell->event.time.tick += q->timer->tick.cur_tick;
			break;

		case SNDRV_SEQ_TIME_STAMP_REAL:
			snd_seq_inc_real_time(&cell->event.time.time,
					      &q->timer->cur_time);
			break;
		}
		cell->event.flags &= ~SNDRV_SEQ_TIME_MODE_MASK;
		cell->event.flags |= SNDRV_SEQ_TIME_MODE_ABS;
	}
	 
	switch (cell->event.flags & SNDRV_SEQ_TIME_STAMP_MASK) {
	case SNDRV_SEQ_TIME_STAMP_TICK:
		err = snd_seq_prioq_cell_in(q->tickq, cell);
		break;

	case SNDRV_SEQ_TIME_STAMP_REAL:
	default:
		err = snd_seq_prioq_cell_in(q->timeq, cell);
		break;
	}

	if (err < 0) {
		queuefree(q);  
		return err;
	}

	 
	snd_seq_check_queue(q, atomic, hop);

	queuefree(q);  

	return 0;
}
