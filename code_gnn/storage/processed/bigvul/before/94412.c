static int sd_revalidate_disk(struct gendisk *disk)
{
	struct scsi_disk *sdkp = scsi_disk(disk);
	struct scsi_device *sdp = sdkp->device;
	unsigned char *buffer;
	unsigned flush = 0;

	SCSI_LOG_HLQUEUE(3, sd_printk(KERN_INFO, sdkp,
				      "sd_revalidate_disk\n"));

	 
	if (!scsi_device_online(sdp))
		goto out;

	buffer = kmalloc(SD_BUF_SIZE, GFP_KERNEL);
	if (!buffer) {
		sd_printk(KERN_WARNING, sdkp, "sd_revalidate_disk: Memory "
			  "allocation failure.\n");
		goto out;
	}

	sd_spinup_disk(sdkp);

	 
	if (sdkp->media_present) {
		sd_read_capacity(sdkp, buffer);

		if (sd_try_extended_inquiry(sdp)) {
			sd_read_block_provisioning(sdkp);
			sd_read_block_limits(sdkp);
			sd_read_block_characteristics(sdkp);
		}

		sd_read_write_protect_flag(sdkp, buffer);
		sd_read_cache_type(sdkp, buffer);
		sd_read_app_tag_own(sdkp, buffer);
	}

	sdkp->first_scan = 0;

	 
	if (sdkp->WCE) {
		flush |= REQ_FLUSH;
		if (sdkp->DPOFUA)
			flush |= REQ_FUA;
	}

	blk_queue_flush(sdkp->disk->queue, flush);

	set_capacity(disk, sdkp->capacity);
	kfree(buffer);

 out:
	return 0;
}
