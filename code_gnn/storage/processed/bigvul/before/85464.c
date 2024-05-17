static int smp_ata_check_ready(struct ata_link *link)
{
	int res;
	struct ata_port *ap = link->ap;
	struct domain_device *dev = ap->private_data;
	struct domain_device *ex_dev = dev->parent;
	struct sas_phy *phy = sas_get_local_phy(dev);
	struct ex_phy *ex_phy = &ex_dev->ex_dev.ex_phy[phy->number];

	res = sas_ex_phy_discover(ex_dev, phy->number);
	sas_put_local_phy(phy);

	 
	if (res == -ECOMM)
		return res;
	if (res != SMP_RESP_FUNC_ACC)
		return 0;

	switch (ex_phy->attached_dev_type) {
	case SAS_SATA_PENDING:
		return 0;
	case SAS_END_DEVICE:
		if (ex_phy->attached_sata_dev)
			return sas_ata_clear_pending(dev, ex_phy);
		 
	default:
		return -ENODEV;
	}
}