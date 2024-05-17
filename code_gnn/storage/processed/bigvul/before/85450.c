static unsigned int sas_ata_qc_issue(struct ata_queued_cmd *qc)
{
	unsigned long flags;
	struct sas_task *task;
	struct scatterlist *sg;
	int ret = AC_ERR_SYSTEM;
	unsigned int si, xfer = 0;
	struct ata_port *ap = qc->ap;
	struct domain_device *dev = ap->private_data;
	struct sas_ha_struct *sas_ha = dev->port->ha;
	struct Scsi_Host *host = sas_ha->core.shost;
	struct sas_internal *i = to_sas_internal(host->transportt);

	 
	local_irq_save(flags);
	spin_unlock(ap->lock);

	 
	if (test_bit(SAS_DEV_GONE, &dev->state))
		goto out;

	task = sas_alloc_task(GFP_ATOMIC);
	if (!task)
		goto out;
	task->dev = dev;
	task->task_proto = SAS_PROTOCOL_STP;
	task->task_done = sas_ata_task_done;

	if (qc->tf.command == ATA_CMD_FPDMA_WRITE ||
	    qc->tf.command == ATA_CMD_FPDMA_READ ||
	    qc->tf.command == ATA_CMD_FPDMA_RECV ||
	    qc->tf.command == ATA_CMD_FPDMA_SEND ||
	    qc->tf.command == ATA_CMD_NCQ_NON_DATA) {
		 
		qc->tf.nsect = 0;
	}

	ata_tf_to_fis(&qc->tf, qc->dev->link->pmp, 1, (u8 *)&task->ata_task.fis);
	task->uldd_task = qc;
	if (ata_is_atapi(qc->tf.protocol)) {
		memcpy(task->ata_task.atapi_packet, qc->cdb, qc->dev->cdb_len);
		task->total_xfer_len = qc->nbytes;
		task->num_scatter = qc->n_elem;
	} else {
		for_each_sg(qc->sg, sg, qc->n_elem, si)
			xfer += sg_dma_len(sg);

		task->total_xfer_len = xfer;
		task->num_scatter = si;
	}

	task->data_dir = qc->dma_dir;
	task->scatter = qc->sg;
	task->ata_task.retry_count = 1;
	task->task_state_flags = SAS_TASK_STATE_PENDING;
	qc->lldd_task = task;

	task->ata_task.use_ncq = ata_is_ncq(qc->tf.protocol);
	task->ata_task.dma_xfer = ata_is_dma(qc->tf.protocol);

	if (qc->scsicmd)
		ASSIGN_SAS_TASK(qc->scsicmd, task);

	ret = i->dft->lldd_execute_task(task, GFP_ATOMIC);
	if (ret) {
		SAS_DPRINTK("lldd_execute_task returned: %d\n", ret);

		if (qc->scsicmd)
			ASSIGN_SAS_TASK(qc->scsicmd, NULL);
		sas_free_task(task);
		qc->lldd_task = NULL;
		ret = AC_ERR_SYSTEM;
	}

 out:
	spin_lock(ap->lock);
	local_irq_restore(flags);
	return ret;
}