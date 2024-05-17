static bool dev_type_flutter(enum sas_device_type new, enum sas_device_type old)
{
	if (old == new)
		return true;

	 
	if ((old == SAS_SATA_PENDING && new == SAS_END_DEVICE) ||
	    (old == SAS_END_DEVICE && new == SAS_SATA_PENDING))
		return true;

	return false;
}
