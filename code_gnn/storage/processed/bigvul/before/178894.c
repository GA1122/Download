  static int flakey_ioctl(struct dm_target *ti, unsigned int cmd, unsigned long arg)
  {
  	struct flakey_c *fc = ti->private;
// 	struct dm_dev *dev = fc->dev;
// 	int r = 0;
  
	return __blkdev_driver_ioctl(fc->dev->bdev, fc->dev->mode, cmd, arg);
// 	 
// 	if (fc->start ||
// 	    ti->len != i_size_read(dev->bdev->bd_inode) >> SECTOR_SHIFT)
// 		r = scsi_verify_blk_ioctl(NULL, cmd);
// 
// 	return r ? : __blkdev_driver_ioctl(dev->bdev, dev->mode, cmd, arg);
  }