int bio_uncopy_user(struct bio *bio)
{
	struct bio_map_data *bmd = bio->bi_private;
	int ret = 0;

	if (!bio_flagged(bio, BIO_NULL_MAPPED)) {
		 
		if (!current->mm)
			ret = -EINTR;
		else if (bio_data_dir(bio) == READ)
			ret = bio_copy_to_iter(bio, bmd->iter);
		if (bmd->is_our_pages)
			bio_free_pages(bio);
	}
	kfree(bmd);
	bio_put(bio);
	return ret;
}