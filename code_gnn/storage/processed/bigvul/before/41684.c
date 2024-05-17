static void btrfs_submit_direct(int rw, struct bio *dio_bio,
				struct inode *inode, loff_t file_offset)
{
	struct btrfs_dio_private *dip = NULL;
	struct bio *io_bio = NULL;
	struct btrfs_io_bio *btrfs_bio;
	int skip_sum;
	int write = rw & REQ_WRITE;
	int ret = 0;

	skip_sum = BTRFS_I(inode)->flags & BTRFS_INODE_NODATASUM;

	io_bio = btrfs_bio_clone(dio_bio, GFP_NOFS);
	if (!io_bio) {
		ret = -ENOMEM;
		goto free_ordered;
	}

	dip = kzalloc(sizeof(*dip), GFP_NOFS);
	if (!dip) {
		ret = -ENOMEM;
		goto free_ordered;
	}

	dip->private = dio_bio->bi_private;
	dip->inode = inode;
	dip->logical_offset = file_offset;
	dip->bytes = dio_bio->bi_iter.bi_size;
	dip->disk_bytenr = (u64)dio_bio->bi_iter.bi_sector << 9;
	io_bio->bi_private = dip;
	dip->orig_bio = io_bio;
	dip->dio_bio = dio_bio;
	atomic_set(&dip->pending_bios, 0);
	btrfs_bio = btrfs_io_bio(io_bio);
	btrfs_bio->logical = file_offset;

	if (write) {
		io_bio->bi_end_io = btrfs_endio_direct_write;
	} else {
		io_bio->bi_end_io = btrfs_endio_direct_read;
		dip->subio_endio = btrfs_subio_endio_read;
	}

	ret = btrfs_submit_direct_hook(rw, dip, skip_sum);
	if (!ret)
		return;

	if (btrfs_bio->end_io)
		btrfs_bio->end_io(btrfs_bio, ret);

free_ordered:
	 
	if (io_bio && dip) {
		io_bio->bi_error = -EIO;
		bio_endio(io_bio);
		 
		dip = NULL;
		io_bio = NULL;
	} else {
		if (write) {
			struct btrfs_ordered_extent *ordered;

			ordered = btrfs_lookup_ordered_extent(inode,
							      file_offset);
			set_bit(BTRFS_ORDERED_IOERR, &ordered->flags);
			 
			btrfs_finish_ordered_io(ordered);
		} else {
			unlock_extent(&BTRFS_I(inode)->io_tree, file_offset,
			      file_offset + dio_bio->bi_iter.bi_size - 1);
		}
		dio_bio->bi_error = -EIO;
		 
		dio_end_io(dio_bio, ret);
	}
	if (io_bio)
		bio_put(io_bio);
	kfree(dip);
}
