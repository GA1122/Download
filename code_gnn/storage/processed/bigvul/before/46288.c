int set_blocksize(struct block_device *bdev, int size)
{
	 
	if (size > PAGE_SIZE || size < 512 || !is_power_of_2(size))
		return -EINVAL;

	 
	if (size < bdev_logical_block_size(bdev))
		return -EINVAL;

	 
	if (bdev->bd_block_size != size) {
		sync_blockdev(bdev);
		bdev->bd_block_size = size;
		bdev->bd_inode->i_blkbits = blksize_bits(size);
		kill_bdev(bdev);
	}
	return 0;
}
