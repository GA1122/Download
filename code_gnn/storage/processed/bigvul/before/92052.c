blk_qc_t generic_make_request(struct bio *bio)
{
	 
	struct bio_list bio_list_on_stack[2];
	blk_mq_req_flags_t flags = 0;
	struct request_queue *q = bio->bi_disk->queue;
	blk_qc_t ret = BLK_QC_T_NONE;

	if (bio->bi_opf & REQ_NOWAIT)
		flags = BLK_MQ_REQ_NOWAIT;
	if (bio_flagged(bio, BIO_QUEUE_ENTERED))
		blk_queue_enter_live(q);
	else if (blk_queue_enter(q, flags) < 0) {
		if (!blk_queue_dying(q) && (bio->bi_opf & REQ_NOWAIT))
			bio_wouldblock_error(bio);
		else
			bio_io_error(bio);
		return ret;
	}

	if (!generic_make_request_checks(bio))
		goto out;

	 
	if (current->bio_list) {
		bio_list_add(&current->bio_list[0], bio);
		goto out;
	}

	 
	BUG_ON(bio->bi_next);
	bio_list_init(&bio_list_on_stack[0]);
	current->bio_list = bio_list_on_stack;
	do {
		bool enter_succeeded = true;

		if (unlikely(q != bio->bi_disk->queue)) {
			if (q)
				blk_queue_exit(q);
			q = bio->bi_disk->queue;
			flags = 0;
			if (bio->bi_opf & REQ_NOWAIT)
				flags = BLK_MQ_REQ_NOWAIT;
			if (blk_queue_enter(q, flags) < 0) {
				enter_succeeded = false;
				q = NULL;
			}
		}

		if (enter_succeeded) {
			struct bio_list lower, same;

			 
			bio_list_on_stack[1] = bio_list_on_stack[0];
			bio_list_init(&bio_list_on_stack[0]);
			ret = q->make_request_fn(q, bio);

			 
			bio_list_init(&lower);
			bio_list_init(&same);
			while ((bio = bio_list_pop(&bio_list_on_stack[0])) != NULL)
				if (q == bio->bi_disk->queue)
					bio_list_add(&same, bio);
				else
					bio_list_add(&lower, bio);
			 
			bio_list_merge(&bio_list_on_stack[0], &lower);
			bio_list_merge(&bio_list_on_stack[0], &same);
			bio_list_merge(&bio_list_on_stack[0], &bio_list_on_stack[1]);
		} else {
			if (unlikely(!blk_queue_dying(q) &&
					(bio->bi_opf & REQ_NOWAIT)))
				bio_wouldblock_error(bio);
			else
				bio_io_error(bio);
		}
		bio = bio_list_pop(&bio_list_on_stack[0]);
	} while (bio);
	current->bio_list = NULL;  

out:
	if (q)
		blk_queue_exit(q);
	return ret;
}
