void md_super_write(struct mddev *mddev, struct md_rdev *rdev,
		   sector_t sector, int size, struct page *page)
{
	 
	struct bio *bio = bio_alloc_mddev(GFP_NOIO, 1, mddev);

	bio->bi_bdev = rdev->meta_bdev ? rdev->meta_bdev : rdev->bdev;
	bio->bi_iter.bi_sector = sector;
	bio_add_page(bio, page, size, 0);
	bio->bi_private = rdev;
	bio->bi_end_io = super_written;

	atomic_inc(&mddev->pending_writes);
	submit_bio(WRITE_FLUSH_FUA, bio);
}