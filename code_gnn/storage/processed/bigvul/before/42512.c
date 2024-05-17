rdev_size_store(struct md_rdev *rdev, const char *buf, size_t len)
{
	struct mddev *my_mddev = rdev->mddev;
	sector_t oldsectors = rdev->sectors;
	sector_t sectors;

	if (strict_blocks_to_sectors(buf, &sectors) < 0)
		return -EINVAL;
	if (rdev->data_offset != rdev->new_data_offset)
		return -EINVAL;  
	if (my_mddev->pers && rdev->raid_disk >= 0) {
		if (my_mddev->persistent) {
			sectors = super_types[my_mddev->major_version].
				rdev_size_change(rdev, sectors);
			if (!sectors)
				return -EBUSY;
		} else if (!sectors)
			sectors = (i_size_read(rdev->bdev->bd_inode) >> 9) -
				rdev->data_offset;
		if (!my_mddev->pers->resize)
			 
			return -EINVAL;
	}
	if (sectors < my_mddev->dev_sectors)
		return -EINVAL;  

	rdev->sectors = sectors;
	if (sectors > oldsectors && my_mddev->external) {
		 
		struct mddev *mddev;
		int overlap = 0;
		struct list_head *tmp;

		rcu_read_lock();
		for_each_mddev(mddev, tmp) {
			struct md_rdev *rdev2;

			rdev_for_each(rdev2, mddev)
				if (rdev->bdev == rdev2->bdev &&
				    rdev != rdev2 &&
				    overlaps(rdev->data_offset, rdev->sectors,
					     rdev2->data_offset,
					     rdev2->sectors)) {
					overlap = 1;
					break;
				}
			if (overlap) {
				mddev_put(mddev);
				break;
			}
		}
		rcu_read_unlock();
		if (overlap) {
			 
			rdev->sectors = oldsectors;
			return -EBUSY;
		}
	}
	return len;
}
