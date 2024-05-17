static void sas_scsi_task_done(struct sas_task *task)
{
	struct scsi_cmnd *sc = task->uldd_task;
	struct domain_device *dev = task->dev;
	struct sas_ha_struct *ha = dev->port->ha;
	unsigned long flags;

	spin_lock_irqsave(&dev->done_lock, flags);
	if (test_bit(SAS_HA_FROZEN, &ha->state))
		task = NULL;
	else
		ASSIGN_SAS_TASK(sc, NULL);
	spin_unlock_irqrestore(&dev->done_lock, flags);

	if (unlikely(!task)) {
		 
		SAS_DPRINTK("task done but aborted\n");
		return;
	}

	if (unlikely(!sc)) {
		SAS_DPRINTK("task_done called with non existing SCSI cmnd!\n");
		sas_free_task(task);
		return;
	}

	sas_end_task(sc, task);
	sc->scsi_done(sc);
}