int bio_add_pc_page(struct request_queue *q, struct bio *bio, struct page
		    *page, unsigned int len, unsigned int offset)
{
	int retried_segments = 0;
	struct bio_vec *bvec;

	 
	if (unlikely(bio_flagged(bio, BIO_CLONED)))
		return 0;

	if (((bio->bi_iter.bi_size + len) >> 9) > queue_max_hw_sectors(q))
		return 0;

	 
	if (bio->bi_vcnt > 0) {
		struct bio_vec *prev = &bio->bi_io_vec[bio->bi_vcnt - 1];

		if (page == prev->bv_page &&
		    offset == prev->bv_offset + prev->bv_len) {
			prev->bv_len += len;
			bio->bi_iter.bi_size += len;
			goto done;
		}

		 
		if (bvec_gap_to_prev(q, prev, offset))
			return 0;
	}

	if (bio->bi_vcnt >= bio->bi_max_vecs)
		return 0;

	 
	bvec = &bio->bi_io_vec[bio->bi_vcnt];
	bvec->bv_page = page;
	bvec->bv_len = len;
	bvec->bv_offset = offset;
	bio->bi_vcnt++;
	bio->bi_phys_segments++;
	bio->bi_iter.bi_size += len;

	 

	while (bio->bi_phys_segments > queue_max_segments(q)) {

		if (retried_segments)
			goto failed;

		retried_segments = 1;
		blk_recount_segments(q, bio);
	}

	 
	if (bio->bi_vcnt > 1 && (BIOVEC_PHYS_MERGEABLE(bvec-1, bvec)))
		bio_clear_flag(bio, BIO_SEG_VALID);

 done:
	return len;

 failed:
	bvec->bv_page = NULL;
	bvec->bv_len = 0;
	bvec->bv_offset = 0;
	bio->bi_vcnt--;
	bio->bi_iter.bi_size -= len;
	blk_recount_segments(q, bio);
	return 0;
}