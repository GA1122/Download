void sas_probe_sata(struct asd_sas_port *port)
{
	struct domain_device *dev, *n;

	mutex_lock(&port->ha->disco_mutex);
	list_for_each_entry(dev, &port->disco_list, disco_list_node) {
		if (!dev_is_sata(dev))
			continue;

		ata_sas_async_probe(dev->sata_dev.ap);
	}
	mutex_unlock(&port->ha->disco_mutex);

	list_for_each_entry_safe(dev, n, &port->disco_list, disco_list_node) {
		if (!dev_is_sata(dev))
			continue;

		sas_ata_wait_eh(dev);

		 
		if (ata_dev_disabled(sas_to_ata_dev(dev)))
			sas_fail_probe(dev, __func__, -ENODEV);
	}

}