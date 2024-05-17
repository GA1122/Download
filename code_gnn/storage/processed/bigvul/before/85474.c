static void sas_suspend_devices(struct work_struct *work)
{
	struct asd_sas_phy *phy;
	struct domain_device *dev;
	struct sas_discovery_event *ev = to_sas_discovery_event(work);
	struct asd_sas_port *port = ev->port;
	struct Scsi_Host *shost = port->ha->core.shost;
	struct sas_internal *si = to_sas_internal(shost->transportt);

	clear_bit(DISCE_SUSPEND, &port->disc.pending);

	sas_suspend_sata(port);

	 
	list_for_each_entry(dev, &port->dev_list, dev_list_node)
		sas_notify_lldd_dev_gone(dev);

	 
	list_for_each_entry(phy, &port->phy_list, port_phy_el) {
		if (si->dft->lldd_port_formed)
			si->dft->lldd_port_deformed(phy);
		phy->suspended = 1;
		port->suspended = 1;
	}
}
