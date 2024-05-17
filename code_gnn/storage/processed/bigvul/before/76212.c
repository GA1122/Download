static int cdrom_dvdram_open_write(struct cdrom_device_info *cdi)
{
	int ret = cdrom_media_erasable(cdi);

	 
	if (!ret)
		return 1;

	return 0;
}
