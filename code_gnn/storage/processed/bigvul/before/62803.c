static void __bio_unmap_user(struct bio *bio)
{
	struct bio_vec *bvec;
	int i;

	 
	bio_for_each_segment_all(bvec, bio, i) {
		if (bio_data_dir(bio) == READ)
			set_page_dirty_lock(bvec->bv_page);

		put_page(bvec->bv_page);
	}

	bio_put(bio);
}
