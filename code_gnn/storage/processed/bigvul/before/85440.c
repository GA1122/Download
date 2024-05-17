static int sas_ata_clear_pending(struct domain_device *dev, struct ex_phy *phy)
{
	int res;

	 
	if (dev->dev_type != SAS_SATA_PENDING)
		return 1;

	 
	res = sas_get_ata_info(dev, phy);
	if (res)
		return 0;  
	else
		return 1;
}
