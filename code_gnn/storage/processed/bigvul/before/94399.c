static void sd_probe_async(void *data, async_cookie_t cookie)
{
	struct scsi_disk *sdkp = data;
	struct scsi_device *sdp;
	struct gendisk *gd;
	u32 index;
	struct device *dev;

	sdp = sdkp->device;
	gd = sdkp->disk;
	index = sdkp->index;
	dev = &sdp->sdev_gendev;

	gd->major = sd_major((index & 0xf0) >> 4);
	gd->first_minor = ((index & 0xf) << 4) | (index & 0xfff00);
	gd->minors = SD_MINORS;

	gd->fops = &sd_fops;
	gd->private_data = &sdkp->driver;
	gd->queue = sdkp->device->request_queue;

	 
	sdp->sector_size = 512;
	sdkp->capacity = 0;
	sdkp->media_present = 1;
	sdkp->write_prot = 0;
	sdkp->WCE = 0;
	sdkp->RCD = 0;
	sdkp->ATO = 0;
	sdkp->first_scan = 1;

	sd_revalidate_disk(gd);

	blk_queue_prep_rq(sdp->request_queue, sd_prep_fn);
	blk_queue_unprep_rq(sdp->request_queue, sd_unprep_fn);

	gd->driverfs_dev = &sdp->sdev_gendev;
	gd->flags = GENHD_FL_EXT_DEVT;
	if (sdp->removable) {
		gd->flags |= GENHD_FL_REMOVABLE;
		gd->events |= DISK_EVENT_MEDIA_CHANGE;
	}

	add_disk(gd);
	sd_dif_config_host(sdkp);

	sd_revalidate_disk(gd);

	sd_printk(KERN_NOTICE, sdkp, "Attached SCSI %sdisk\n",
		  sdp->removable ? "removable " : "");
	scsi_autopm_put_device(sdp);
	put_device(&sdkp->dev);
}