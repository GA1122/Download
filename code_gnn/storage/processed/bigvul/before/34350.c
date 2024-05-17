static void btrfs_submit_direct(int rw, struct bio *bio, struct inode *inode,
				loff_t file_offset)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_dio_private *dip;
	struct bio_vec *bvec = bio->bi_io_vec;
	int skip_sum;
	int write = rw & REQ_WRITE;
	int ret = 0;

	skip_sum = BTRFS_I(inode)->flags & BTRFS_INODE_NODATASUM;

	dip = kmalloc(sizeof(*dip), GFP_NOFS);
	if (!dip) {
		ret = -ENOMEM;
		goto free_ordered;
	}

	dip->private = bio->bi_private;
	dip->inode = inode;
	dip->logical_offset = file_offset;

	dip->bytes = 0;
	do {
		dip->bytes += bvec->bv_len;
		bvec++;
	} while (bvec <= (bio->bi_io_vec + bio->bi_vcnt - 1));

	dip->disk_bytenr = (u64)bio->bi_sector << 9;
	bio->bi_private = dip;
	dip->errors = 0;
	dip->orig_bio = bio;
	atomic_set(&dip->pending_bios, 0);

	if (write)
		bio->bi_end_io = btrfs_endio_direct_write;
	else
		bio->bi_end_io = btrfs_endio_direct_read;

	ret = btrfs_submit_direct_hook(rw, dip, skip_sum);
	if (!ret)
		return;
free_ordered:
	 
	if (write) {
		struct btrfs_ordered_extent *ordered;
		ordered = btrfs_lookup_ordered_extent(inode, file_offset);
		if (!test_bit(BTRFS_ORDERED_PREALLOC, &ordered->flags) &&
		    !test_bit(BTRFS_ORDERED_NOCOW, &ordered->flags))
			btrfs_free_reserved_extent(root, ordered->start,
						   ordered->disk_len);
		btrfs_put_ordered_extent(ordered);
		btrfs_put_ordered_extent(ordered);
	}
	bio_endio(bio, ret);
}