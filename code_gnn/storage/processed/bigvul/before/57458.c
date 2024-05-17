static int ext4_ext_zeroout(struct inode *inode, struct ext4_extent *ex)
{
	int ret = -EIO;
	struct bio *bio;
	int blkbits, blocksize;
	sector_t ee_pblock;
	struct completion event;
	unsigned int ee_len, len, done, offset;


	blkbits   = inode->i_blkbits;
	blocksize = inode->i_sb->s_blocksize;
	ee_len    = ext4_ext_get_actual_len(ex);
	ee_pblock = ext_pblock(ex);

	 
	ee_pblock = ee_pblock << (blkbits - 9);

	while (ee_len > 0) {

		if (ee_len > BIO_MAX_PAGES)
			len = BIO_MAX_PAGES;
		else
			len = ee_len;

		bio = bio_alloc(GFP_NOIO, len);
		bio->bi_sector = ee_pblock;
		bio->bi_bdev   = inode->i_sb->s_bdev;

		done = 0;
		offset = 0;
		while (done < len) {
			ret = bio_add_page(bio, ZERO_PAGE(0),
							blocksize, offset);
			if (ret != blocksize) {
				 
				break;
			}
			done++;
			offset += blocksize;
			if (offset >= PAGE_CACHE_SIZE)
				offset = 0;
		}

		init_completion(&event);
		bio->bi_private = &event;
		bio->bi_end_io = bi_complete;
		submit_bio(WRITE, bio);
		wait_for_completion(&event);

		if (test_bit(BIO_UPTODATE, &bio->bi_flags))
			ret = 0;
		else {
			ret = -EIO;
			break;
		}
		bio_put(bio);
		ee_len    -= done;
		ee_pblock += done  << (blkbits - 9);
	}
	return ret;
}