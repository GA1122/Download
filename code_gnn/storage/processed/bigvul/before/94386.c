static unsigned int sd_check_events(struct gendisk *disk, unsigned int clearing)
{
	struct scsi_disk *sdkp = scsi_disk(disk);
	struct scsi_device *sdp = sdkp->device;
	struct scsi_sense_hdr *sshdr = NULL;
	int retval;

	SCSI_LOG_HLQUEUE(3, sd_printk(KERN_INFO, sdkp, "sd_check_events\n"));

	 
	if (!scsi_device_online(sdp)) {
		set_media_not_present(sdkp);
		goto out;
	}

	 
	retval = -ENODEV;

	if (scsi_block_when_processing_errors(sdp)) {
		sshdr  = kzalloc(sizeof(*sshdr), GFP_KERNEL);
		retval = scsi_test_unit_ready(sdp, SD_TIMEOUT, SD_MAX_RETRIES,
					      sshdr);
	}

	 
	if (host_byte(retval)) {
		set_media_not_present(sdkp);
		goto out;
	}

	if (media_not_present(sdkp, sshdr))
		goto out;

	 
	if (!sdkp->media_present)
		sdp->changed = 1;
	sdkp->media_present = 1;
out:
	 
	kfree(sshdr);
	retval = sdp->changed ? DISK_EVENT_MEDIA_CHANGE : 0;
	sdp->changed = 0;
	return retval;
}
