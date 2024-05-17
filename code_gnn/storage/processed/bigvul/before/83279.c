static void sas_wait_eh(struct domain_device *dev)
{
	struct sas_ha_struct *ha = dev->port->ha;
	DEFINE_WAIT(wait);

	if (dev_is_sata(dev)) {
		ata_port_wait_eh(dev->sata_dev.ap);
		return;
	}
 retry:
	spin_lock_irq(&ha->lock);

	while (test_bit(SAS_DEV_EH_PENDING, &dev->state)) {
		prepare_to_wait(&ha->eh_wait_q, &wait, TASK_UNINTERRUPTIBLE);
		spin_unlock_irq(&ha->lock);
		schedule();
		spin_lock_irq(&ha->lock);
	}
	finish_wait(&ha->eh_wait_q, &wait);

	spin_unlock_irq(&ha->lock);

	 
	if (scsi_host_in_recovery(ha->core.shost)) {
		msleep(10);
		goto retry;
	}
}