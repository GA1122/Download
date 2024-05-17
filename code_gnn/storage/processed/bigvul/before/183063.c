 static int sd_ioctl(struct block_device *bdev, fmode_t mode,
 		    unsigned int cmd, unsigned long arg)
 {
 	struct gendisk *disk = bdev->bd_disk;
 	struct scsi_disk *sdkp = scsi_disk(disk);
 	struct scsi_device *sdp = sdkp->device;
 	void __user *p = (void __user *)arg;
 	int error;
     
  	SCSI_LOG_IOCTL(1, sd_printk(KERN_INFO, sdkp, "sd_ioctl: disk=%s, "
  				    "cmd=0x%x\n", disk->disk_name, cmd));
  
// 	error = scsi_verify_blk_ioctl(bdev, cmd);
// 	if (error < 0)
// 		return error;
// 
  	 
 	error = scsi_nonblockable_ioctl(sdp, cmd, p,
 					(mode & FMODE_NDELAY) != 0);
 	if (!scsi_block_when_processing_errors(sdp) || !error)
 		goto out;
 
 	 
 	switch (cmd) {
 		case SCSI_IOCTL_GET_IDLUN:
 		case SCSI_IOCTL_GET_BUS_NUMBER:
 			error = scsi_ioctl(sdp, cmd, p);
 			break;
 		default:
 			error = scsi_cmd_blk_ioctl(bdev, mode, cmd, p);
 			if (error != -ENOTTY)
 				break;
 			error = scsi_ioctl(sdp, cmd, p);
 			break;
 	}
 out:
 	return error;
 }