static int update_size(struct mddev *mddev, sector_t num_sectors)
{
	struct md_rdev *rdev;
	int rv;
	int fit = (num_sectors == 0);

	if (mddev->pers->resize == NULL)
		return -EINVAL;
	 
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) ||
	    mddev->sync_thread)
		return -EBUSY;
	if (mddev->ro)
		return -EROFS;

	rdev_for_each(rdev, mddev) {
		sector_t avail = rdev->sectors;

		if (fit && (num_sectors == 0 || num_sectors > avail))
			num_sectors = avail;
		if (avail < num_sectors)
			return -ENOSPC;
	}
	rv = mddev->pers->resize(mddev, num_sectors);
	if (!rv)
		revalidate_disk(mddev->gendisk);
	return rv;
}