int bio_add_page(struct bio *bio, struct page *page,
		 unsigned int len, unsigned int offset)
{
	struct bio_vec *bv;

	 
	if (WARN_ON_ONCE(bio_flagged(bio, BIO_CLONED)))
		return 0;

	 
	if (bio->bi_vcnt > 0) {
		bv = &bio->bi_io_vec[bio->bi_vcnt - 1];

		if (page == bv->bv_page &&
		    offset == bv->bv_offset + bv->bv_len) {
			bv->bv_len += len;
			goto done;
		}
	}

	if (bio->bi_vcnt >= bio->bi_max_vecs)
		return 0;

	bv		= &bio->bi_io_vec[bio->bi_vcnt];
	bv->bv_page	= page;
	bv->bv_len	= len;
	bv->bv_offset	= offset;

	bio->bi_vcnt++;
done:
	bio->bi_iter.bi_size += len;
	return len;
}
