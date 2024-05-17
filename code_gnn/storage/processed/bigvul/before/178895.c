 static int linear_ioctl(struct dm_target *ti, unsigned int cmd,
  			unsigned long arg)
  {
  	struct linear_c *lc = (struct linear_c *) ti->private;
	return __blkdev_driver_ioctl(lc->dev->bdev, lc->dev->mode, cmd, arg);
// 	struct dm_dev *dev = lc->dev;
// 	int r = 0;
// 
// 	 
// 	if (lc->start ||
// 	    ti->len != i_size_read(dev->bdev->bd_inode) >> SECTOR_SHIFT)
// 		r = scsi_verify_blk_ioctl(NULL, cmd);
// 
// 	return r ? : __blkdev_driver_ioctl(dev->bdev, dev->mode, cmd, arg);
  }