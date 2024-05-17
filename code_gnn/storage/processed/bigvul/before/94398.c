static int sd_probe(struct device *dev)
{
	struct scsi_device *sdp = to_scsi_device(dev);
	struct scsi_disk *sdkp;
	struct gendisk *gd;
	int index;
	int error;

	error = -ENODEV;
	if (sdp->type != TYPE_DISK && sdp->type != TYPE_MOD && sdp->type != TYPE_RBC)
		goto out;

	SCSI_LOG_HLQUEUE(3, sdev_printk(KERN_INFO, sdp,
					"sd_attach\n"));

	error = -ENOMEM;
	sdkp = kzalloc(sizeof(*sdkp), GFP_KERNEL);
	if (!sdkp)
		goto out;

	gd = alloc_disk(SD_MINORS);
	if (!gd)
		goto out_free;

	do {
		if (!ida_pre_get(&sd_index_ida, GFP_KERNEL))
			goto out_put;

		spin_lock(&sd_index_lock);
		error = ida_get_new(&sd_index_ida, &index);
		spin_unlock(&sd_index_lock);
	} while (error == -EAGAIN);

	if (error) {
		sdev_printk(KERN_WARNING, sdp, "sd_probe: memory exhausted.\n");
		goto out_put;
	}

	error = sd_format_disk_name("sd", index, gd->disk_name, DISK_NAME_LEN);
	if (error) {
		sdev_printk(KERN_WARNING, sdp, "SCSI disk (sd) name length exceeded.\n");
		goto out_free_index;
	}

	sdkp->device = sdp;
	sdkp->driver = &sd_template;
	sdkp->disk = gd;
	sdkp->index = index;
	atomic_set(&sdkp->openers, 0);

	if (!sdp->request_queue->rq_timeout) {
		if (sdp->type != TYPE_MOD)
			blk_queue_rq_timeout(sdp->request_queue, SD_TIMEOUT);
		else
			blk_queue_rq_timeout(sdp->request_queue,
					     SD_MOD_TIMEOUT);
	}

	device_initialize(&sdkp->dev);
	sdkp->dev.parent = dev;
	sdkp->dev.class = &sd_disk_class;
	dev_set_name(&sdkp->dev, dev_name(dev));

	if (device_add(&sdkp->dev))
		goto out_free_index;

	get_device(dev);
	dev_set_drvdata(dev, sdkp);

	get_device(&sdkp->dev);	 
	async_schedule(sd_probe_async, sdkp);

	return 0;

 out_free_index:
	spin_lock(&sd_index_lock);
	ida_remove(&sd_index_ida, index);
	spin_unlock(&sd_index_lock);
 out_put:
	put_disk(gd);
 out_free:
	kfree(sdkp);
 out:
	return error;
}