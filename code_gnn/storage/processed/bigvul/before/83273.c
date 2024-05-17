void sas_scsi_recover_host(struct Scsi_Host *shost)
{
	struct sas_ha_struct *ha = SHOST_TO_SAS_HA(shost);
	LIST_HEAD(eh_work_q);
	int tries = 0;
	bool retry;

retry:
	tries++;
	retry = true;
	spin_lock_irq(shost->host_lock);
	list_splice_init(&shost->eh_cmd_q, &eh_work_q);
	spin_unlock_irq(shost->host_lock);

	SAS_DPRINTK("Enter %s busy: %d failed: %d\n",
		    __func__, atomic_read(&shost->host_busy), shost->host_failed);
	 
	set_bit(SAS_HA_FROZEN, &ha->state);
	sas_eh_handle_sas_errors(shost, &eh_work_q);
	clear_bit(SAS_HA_FROZEN, &ha->state);
	if (list_empty(&eh_work_q))
		goto out;

	 
	sas_ata_eh(shost, &eh_work_q, &ha->eh_done_q);
	if (!scsi_eh_get_sense(&eh_work_q, &ha->eh_done_q))
		scsi_eh_ready_devs(shost, &eh_work_q, &ha->eh_done_q);

out:
	sas_eh_handle_resets(shost);

	 
	sas_ata_strategy_handler(shost);

	scsi_eh_flush_done_q(&ha->eh_done_q);

	 
	spin_lock_irq(&ha->lock);
	if (ha->eh_active == 0) {
		shost->host_eh_scheduled = 0;
		retry = false;
	}
	spin_unlock_irq(&ha->lock);

	if (retry)
		goto retry;

	SAS_DPRINTK("--- Exit %s: busy: %d failed: %d tries: %d\n",
		    __func__, atomic_read(&shost->host_busy),
		    shost->host_failed, tries);
}
