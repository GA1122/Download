static void btrfs_end_dio_bio(struct bio *bio, int err)
{
	struct btrfs_dio_private *dip = bio->bi_private;

	if (err) {
		printk(KERN_ERR "btrfs direct IO failed ino %llu rw %lu "
		      "sector %#Lx len %u err no %d\n",
		      (unsigned long long)btrfs_ino(dip->inode), bio->bi_rw,
		      (unsigned long long)bio->bi_sector, bio->bi_size, err);
		dip->errors = 1;

		 
		smp_mb__before_atomic_dec();
	}

	 
	if (!atomic_dec_and_test(&dip->pending_bios))
		goto out;

	if (dip->errors)
		bio_io_error(dip->orig_bio);
	else {
		set_bit(BIO_UPTODATE, &dip->orig_bio->bi_flags);
		bio_endio(dip->orig_bio, 0);
	}
out:
	bio_put(bio);
}