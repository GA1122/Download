struct super_block *freeze_bdev(struct block_device *bdev)
{
	struct super_block *sb;
	int error = 0;

	mutex_lock(&bdev->bd_fsfreeze_mutex);
	if (++bdev->bd_fsfreeze_count > 1) {
		 
		sb = get_super(bdev);
		drop_super(sb);
		mutex_unlock(&bdev->bd_fsfreeze_mutex);
		return sb;
	}

	sb = get_active_super(bdev);
	if (!sb)
		goto out;
	error = freeze_super(sb);
	if (error) {
		deactivate_super(sb);
		bdev->bd_fsfreeze_count--;
		mutex_unlock(&bdev->bd_fsfreeze_mutex);
		return ERR_PTR(error);
	}
	deactivate_super(sb);
 out:
	sync_blockdev(bdev);
	mutex_unlock(&bdev->bd_fsfreeze_mutex);
	return sb;	 
}