static void sas_ata_task_done(struct sas_task *task)
{
	struct ata_queued_cmd *qc = task->uldd_task;
	struct domain_device *dev = task->dev;
	struct task_status_struct *stat = &task->task_status;
	struct ata_task_resp *resp = (struct ata_task_resp *)stat->buf;
	struct sas_ha_struct *sas_ha = dev->port->ha;
	enum ata_completion_errors ac;
	unsigned long flags;
	struct ata_link *link;
	struct ata_port *ap;

	spin_lock_irqsave(&dev->done_lock, flags);
	if (test_bit(SAS_HA_FROZEN, &sas_ha->state))
		task = NULL;
	else if (qc && qc->scsicmd)
		ASSIGN_SAS_TASK(qc->scsicmd, NULL);
	spin_unlock_irqrestore(&dev->done_lock, flags);

	 
	if (unlikely(!task))
		return;

	if (!qc)
		goto qc_already_gone;

	ap = qc->ap;
	link = &ap->link;

	spin_lock_irqsave(ap->lock, flags);
	 
	if (unlikely(ap->pflags & ATA_PFLAG_FROZEN)) {
		spin_unlock_irqrestore(ap->lock, flags);
		if (qc->scsicmd)
			goto qc_already_gone;
		else {
			 
			return;
		}
	}

	if (stat->stat == SAS_PROTO_RESPONSE || stat->stat == SAM_STAT_GOOD ||
	    ((stat->stat == SAM_STAT_CHECK_CONDITION &&
	      dev->sata_dev.class == ATA_DEV_ATAPI))) {
		memcpy(dev->sata_dev.fis, resp->ending_fis, ATA_RESP_FIS_SIZE);

		if (!link->sactive) {
			qc->err_mask |= ac_err_mask(dev->sata_dev.fis[2]);
		} else {
			link->eh_info.err_mask |= ac_err_mask(dev->sata_dev.fis[2]);
			if (unlikely(link->eh_info.err_mask))
				qc->flags |= ATA_QCFLAG_FAILED;
		}
	} else {
		ac = sas_to_ata_err(stat);
		if (ac) {
			SAS_DPRINTK("%s: SAS error %x\n", __func__,
				    stat->stat);
			 
			if (!link->sactive) {
				qc->err_mask = ac;
			} else {
				link->eh_info.err_mask |= AC_ERR_DEV;
				qc->flags |= ATA_QCFLAG_FAILED;
			}

			dev->sata_dev.fis[3] = 0x04;  
			dev->sata_dev.fis[2] = ATA_ERR;
		}
	}

	qc->lldd_task = NULL;
	ata_qc_complete(qc);
	spin_unlock_irqrestore(ap->lock, flags);

qc_already_gone:
	sas_free_task(task);
}