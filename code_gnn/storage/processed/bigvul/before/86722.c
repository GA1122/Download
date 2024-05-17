static void blk_mq_make_request(struct request_queue *q, struct bio *bio)
{
	const int is_sync = rw_is_sync(bio->bi_rw);
	const int is_flush_fua = bio->bi_rw & (REQ_FLUSH | REQ_FUA);
	struct blk_map_ctx data;
	struct request *rq;
	unsigned int request_count = 0;
	struct blk_plug *plug;
	struct request *same_queue_rq = NULL;

	blk_queue_bounce(q, &bio);

	if (bio_integrity_enabled(bio) && bio_integrity_prep(bio)) {
		bio_io_error(bio);
		return;
	}

	blk_queue_split(q, &bio, q->bio_split);

	if (!is_flush_fua && !blk_queue_nomerges(q) &&
	    blk_attempt_plug_merge(q, bio, &request_count, &same_queue_rq))
		return;

	rq = blk_mq_map_request(q, bio, &data);
	if (unlikely(!rq))
		return;

	if (unlikely(is_flush_fua)) {
		blk_mq_bio_to_request(rq, bio);
		blk_insert_flush(rq);
		goto run_queue;
	}

	plug = current->plug;
	 
	if (((plug && !blk_queue_nomerges(q)) || is_sync) &&
	    !(data.hctx->flags & BLK_MQ_F_DEFER_ISSUE)) {
		struct request *old_rq = NULL;

		blk_mq_bio_to_request(rq, bio);

		 
		if (plug) {
			 
			if (same_queue_rq && !list_empty(&plug->mq_list)) {
				old_rq = same_queue_rq;
				list_del_init(&old_rq->queuelist);
			}
			list_add_tail(&rq->queuelist, &plug->mq_list);
		} else  
			old_rq = rq;
		blk_mq_put_ctx(data.ctx);
		if (!old_rq)
			return;
		if (!blk_mq_direct_issue_request(old_rq))
			return;
		blk_mq_insert_request(old_rq, false, true, true);
		return;
	}

	if (!blk_mq_merge_queue_io(data.hctx, data.ctx, rq, bio)) {
		 
run_queue:
		blk_mq_run_hw_queue(data.hctx, !is_sync || is_flush_fua);
	}
	blk_mq_put_ctx(data.ctx);
}