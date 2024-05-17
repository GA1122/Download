void blkdev_put(struct block_device *bdev, fmode_t mode)
{
	mutex_lock(&bdev->bd_mutex);

	if (mode & FMODE_EXCL) {
		bool bdev_free;

		 
		spin_lock(&bdev_lock);

		WARN_ON_ONCE(--bdev->bd_holders < 0);
		WARN_ON_ONCE(--bdev->bd_contains->bd_holders < 0);

		 
		if ((bdev_free = !bdev->bd_holders))
			bdev->bd_holder = NULL;
		if (!bdev->bd_contains->bd_holders)
			bdev->bd_contains->bd_holder = NULL;

		spin_unlock(&bdev_lock);

		 
		if (bdev_free && bdev->bd_write_holder) {
			disk_unblock_events(bdev->bd_disk);
			bdev->bd_write_holder = false;
		}
	}

	 
	disk_flush_events(bdev->bd_disk, DISK_EVENT_MEDIA_CHANGE);

	mutex_unlock(&bdev->bd_mutex);

	__blkdev_put(bdev, mode, 0);
}
