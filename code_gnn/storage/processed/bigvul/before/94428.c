static int sd_sync_cache(struct scsi_disk *sdkp)
{
	int retries, res;
	struct scsi_device *sdp = sdkp->device;
	struct scsi_sense_hdr sshdr;

	if (!scsi_device_online(sdp))
		return -ENODEV;


	for (retries = 3; retries > 0; --retries) {
		unsigned char cmd[10] = { 0 };

		cmd[0] = SYNCHRONIZE_CACHE;
		 
		res = scsi_execute_req(sdp, cmd, DMA_NONE, NULL, 0, &sshdr,
				       SD_FLUSH_TIMEOUT, SD_MAX_RETRIES, NULL);
		if (res == 0)
			break;
	}

	if (res) {
		sd_print_result(sdkp, res);
		if (driver_byte(res) & DRIVER_SENSE)
			sd_print_sense_hdr(sdkp, &sshdr);
	}

	if (res)
		return -EIO;
	return 0;
}