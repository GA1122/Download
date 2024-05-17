static void sas_eh_handle_resets(struct Scsi_Host *shost)
{
	struct sas_ha_struct *ha = SHOST_TO_SAS_HA(shost);
	struct sas_internal *i = to_sas_internal(shost->transportt);

	 
	spin_lock_irq(&ha->lock);
	while (!list_empty(&ha->eh_dev_q)) {
		struct domain_device *dev;
		struct ssp_device *ssp;

		ssp = list_entry(ha->eh_dev_q.next, typeof(*ssp), eh_list_node);
		list_del_init(&ssp->eh_list_node);
		dev = container_of(ssp, typeof(*dev), ssp_dev);
		kref_get(&dev->kref);
		WARN_ONCE(dev_is_sata(dev), "ssp reset to ata device?\n");

		spin_unlock_irq(&ha->lock);

		if (test_and_clear_bit(SAS_DEV_LU_RESET, &dev->state))
			i->dft->lldd_lu_reset(dev, ssp->reset_lun.scsi_lun);

		if (test_and_clear_bit(SAS_DEV_RESET, &dev->state))
			i->dft->lldd_I_T_nexus_reset(dev);

		sas_put_device(dev);
		spin_lock_irq(&ha->lock);
		clear_bit(SAS_DEV_EH_PENDING, &dev->state);
		ha->eh_active--;
	}
	spin_unlock_irq(&ha->lock);
}