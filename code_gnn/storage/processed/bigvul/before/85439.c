static int local_ata_check_ready(struct ata_link *link)
{
	struct ata_port *ap = link->ap;
	struct domain_device *dev = ap->private_data;
	struct sas_internal *i = dev_to_sas_internal(dev);

	if (i->dft->lldd_ata_check_ready)
		return i->dft->lldd_ata_check_ready(dev);
	else {
		 
		return 1;
	}
}