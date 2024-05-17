static void sd_read_protection_type(struct scsi_disk *sdkp, unsigned char *buffer)
{
	struct scsi_device *sdp = sdkp->device;
	u8 type;

	if (scsi_device_protection(sdp) == 0 || (buffer[12] & 1) == 0)
		return;

	type = ((buffer[12] >> 1) & 7) + 1;  

	if (type == sdkp->protection_type || !sdkp->first_scan)
		return;

	sdkp->protection_type = type;

	if (type > SD_DIF_TYPE3_PROTECTION) {
		sd_printk(KERN_ERR, sdkp, "formatted with unsupported "	\
			  "protection type %u. Disabling disk!\n", type);
		sdkp->capacity = 0;
		return;
	}

	if (scsi_host_dif_capable(sdp->host, type))
		sd_printk(KERN_NOTICE, sdkp,
			  "Enabling DIF Type %u protection\n", type);
	else
		sd_printk(KERN_NOTICE, sdkp,
			  "Disabling DIF Type %u protection\n", type);
}