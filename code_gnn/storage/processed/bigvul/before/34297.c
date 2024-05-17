static void btrfs_endio_direct_write(struct bio *bio, int err)
{
	struct btrfs_dio_private *dip = bio->bi_private;
	struct inode *inode = dip->inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_ordered_extent *ordered = NULL;
	u64 ordered_offset = dip->logical_offset;
	u64 ordered_bytes = dip->bytes;
	int ret;

	if (err)
		goto out_done;
again:
	ret = btrfs_dec_test_first_ordered_pending(inode, &ordered,
						   &ordered_offset,
						   ordered_bytes, !err);
	if (!ret)
		goto out_test;

	ordered->work.func = finish_ordered_fn;
	ordered->work.flags = 0;
	btrfs_queue_worker(&root->fs_info->endio_write_workers,
			   &ordered->work);
out_test:
	 
	if (ordered_offset < dip->logical_offset + dip->bytes) {
		ordered_bytes = dip->logical_offset + dip->bytes -
			ordered_offset;
		ordered = NULL;
		goto again;
	}
out_done:
	bio->bi_private = dip->private;

	kfree(dip);

	 
	if (err)
		clear_bit(BIO_UPTODATE, &bio->bi_flags);
	dio_end_io(bio, err);
}