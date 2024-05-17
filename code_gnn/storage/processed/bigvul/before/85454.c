void sas_ata_strategy_handler(struct Scsi_Host *shost)
{
	struct sas_ha_struct *sas_ha = SHOST_TO_SAS_HA(shost);
	ASYNC_DOMAIN_EXCLUSIVE(async);
	int i;

	 
	sas_disable_revalidation(sas_ha);

	spin_lock_irq(&sas_ha->phy_port_lock);
	for (i = 0; i < sas_ha->num_phys; i++) {
		struct asd_sas_port *port = sas_ha->sas_port[i];
		struct domain_device *dev;

		spin_lock(&port->dev_list_lock);
		list_for_each_entry(dev, &port->dev_list, dev_list_node) {
			if (!dev_is_sata(dev))
				continue;

			 
			kref_get(&dev->kref);

			async_schedule_domain(async_sas_ata_eh, dev, &async);
		}
		spin_unlock(&port->dev_list_lock);
	}
	spin_unlock_irq(&sas_ha->phy_port_lock);

	async_synchronize_full_domain(&async);

	sas_enable_revalidation(sas_ha);
}
