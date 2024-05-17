static int cdrom_is_dvd_rw(struct cdrom_device_info *cdi)
{
	switch (cdi->mmc3_profile) {
	case 0x12:	 
	case 0x1A:	 
	case 0x43:	 
		return 0;
	default:
		return 1;
	}
}
