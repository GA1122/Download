void sas_task_abort(struct sas_task *task)
{
	struct scsi_cmnd *sc = task->uldd_task;

	 
	if (!sc) {
		struct sas_task_slow *slow = task->slow_task;

		if (!slow)
			return;
		if (!del_timer(&slow->timer))
			return;
		slow->timer.function(&slow->timer);
		return;
	}

	if (dev_is_sata(task->dev)) {
		sas_ata_task_abort(task);
	} else {
		struct request_queue *q = sc->device->request_queue;
		unsigned long flags;

		spin_lock_irqsave(q->queue_lock, flags);
		blk_abort_request(sc->request);
		spin_unlock_irqrestore(q->queue_lock, flags);
	}
}