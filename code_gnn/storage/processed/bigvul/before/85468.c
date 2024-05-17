void sas_free_device(struct kref *kref)
{
	struct domain_device *dev = container_of(kref, typeof(*dev), kref);

	put_device(&dev->rphy->dev);
	dev->rphy = NULL;

	if (dev->parent)
		sas_put_device(dev->parent);

	sas_port_put_phy(dev->phy);
	dev->phy = NULL;

	 
	if (dev->dev_type == SAS_EDGE_EXPANDER_DEVICE || dev->dev_type == SAS_FANOUT_EXPANDER_DEVICE)
		kfree(dev->ex_dev.ex_phy);

	if (dev_is_sata(dev) && dev->sata_dev.ap) {
		ata_sas_port_destroy(dev->sata_dev.ap);
		dev->sata_dev.ap = NULL;
	}

	kfree(dev);
}