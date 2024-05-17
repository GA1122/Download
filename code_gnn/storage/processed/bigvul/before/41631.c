static void btrfs_end_dio_bio(struct bio *bio)
{
	struct btrfs_dio_private *dip = bio->bi_private;
	int err = bio->bi_error;

	if (err)
		btrfs_warn(BTRFS_I(dip->inode)->root->fs_info,
			   "direct IO failed ino %llu rw %lu sector %#Lx len %u err no %d",
			   btrfs_ino(dip->inode), bio->bi_rw,
			   (unsigned long long)bio->bi_iter.bi_sector,
			   bio->bi_iter.bi_size, err);

	if (dip->subio_endio)
		err = dip->subio_endio(dip->inode, btrfs_io_bio(bio), err);

	if (err) {
		dip->errors = 1;

		 
		smp_mb__before_atomic();
	}

	 
	if (!atomic_dec_and_test(&dip->pending_bios))
		goto out;

	if (dip->errors) {
		bio_io_error(dip->orig_bio);
	} else {
		dip->dio_bio->bi_error = 0;
		bio_endio(dip->orig_bio);
	}
out:
	bio_put(bio);
}
