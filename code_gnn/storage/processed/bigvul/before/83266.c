static int sas_queue_reset(struct domain_device *dev, int reset_type,
			   u64 lun, int wait)
{
	struct sas_ha_struct *ha = dev->port->ha;
	int scheduled = 0, tries = 100;

	 
	if (dev_is_sata(dev)) {
		sas_ata_schedule_reset(dev);
		if (wait)
			sas_ata_wait_eh(dev);
		return SUCCESS;
	}

	while (!scheduled && tries--) {
		spin_lock_irq(&ha->lock);
		if (!test_bit(SAS_DEV_EH_PENDING, &dev->state) &&
		    !test_bit(reset_type, &dev->state)) {
			scheduled = 1;
			ha->eh_active++;
			list_add_tail(&dev->ssp_dev.eh_list_node, &ha->eh_dev_q);
			set_bit(SAS_DEV_EH_PENDING, &dev->state);
			set_bit(reset_type, &dev->state);
			int_to_scsilun(lun, &dev->ssp_dev.reset_lun);
			scsi_schedule_eh(ha->core.shost);
		}
		spin_unlock_irq(&ha->lock);

		if (wait)
			sas_wait_eh(dev);

		if (scheduled)
			return SUCCESS;
	}

	SAS_DPRINTK("%s reset of %s failed\n",
		    reset_type == SAS_DEV_LU_RESET ? "LUN" : "Bus",
		    dev_name(&dev->rphy->dev));

	return FAILED;
}