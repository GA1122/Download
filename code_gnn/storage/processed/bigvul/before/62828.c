void bio_endio(struct bio *bio)
{
again:
	if (!bio_remaining_done(bio))
		return;
	if (!bio_integrity_endio(bio))
		return;

	 
	if (bio->bi_end_io == bio_chain_endio) {
		bio = __bio_chain_endio(bio);
		goto again;
	}

	if (bio->bi_disk && bio_flagged(bio, BIO_TRACE_COMPLETION)) {
		trace_block_bio_complete(bio->bi_disk->queue, bio,
					 blk_status_to_errno(bio->bi_status));
		bio_clear_flag(bio, BIO_TRACE_COMPLETION);
	}

	blk_throtl_bio_endio(bio);
	 
	bio_uninit(bio);
	if (bio->bi_end_io)
		bio->bi_end_io(bio);
}
