static int blkdev_open(struct inode * inode, struct file * filp)
{
	struct block_device *bdev;

	 
	filp->f_flags |= O_LARGEFILE;

	if (filp->f_flags & O_NDELAY)
		filp->f_mode |= FMODE_NDELAY;
	if (filp->f_flags & O_EXCL)
		filp->f_mode |= FMODE_EXCL;
	if ((filp->f_flags & O_ACCMODE) == 3)
		filp->f_mode |= FMODE_WRITE_IOCTL;

	bdev = bd_acquire(inode);
	if (bdev == NULL)
		return -ENOMEM;

	filp->f_mapping = bdev->bd_inode->i_mapping;

	return blkdev_get(bdev, filp->f_mode, filp);
}