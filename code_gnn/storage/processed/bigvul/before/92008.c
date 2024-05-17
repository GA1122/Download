static blk_qc_t blk_queue_bio(struct request_queue *q, struct bio *bio)
{
	struct blk_plug *plug;
	int where = ELEVATOR_INSERT_SORT;
	struct request *req, *free;
	unsigned int request_count = 0;

	 
	blk_queue_bounce(q, &bio);

	blk_queue_split(q, &bio);

	if (!bio_integrity_prep(bio))
		return BLK_QC_T_NONE;

	if (op_is_flush(bio->bi_opf)) {
		spin_lock_irq(q->queue_lock);
		where = ELEVATOR_INSERT_FLUSH;
		goto get_rq;
	}

	 
	if (!blk_queue_nomerges(q)) {
		if (blk_attempt_plug_merge(q, bio, &request_count, NULL))
			return BLK_QC_T_NONE;
	} else
		request_count = blk_plug_queued_count(q);

	spin_lock_irq(q->queue_lock);

	switch (elv_merge(q, &req, bio)) {
	case ELEVATOR_BACK_MERGE:
		if (!bio_attempt_back_merge(q, req, bio))
			break;
		elv_bio_merged(q, req, bio);
		free = attempt_back_merge(q, req);
		if (free)
			__blk_put_request(q, free);
		else
			elv_merged_request(q, req, ELEVATOR_BACK_MERGE);
		goto out_unlock;
	case ELEVATOR_FRONT_MERGE:
		if (!bio_attempt_front_merge(q, req, bio))
			break;
		elv_bio_merged(q, req, bio);
		free = attempt_front_merge(q, req);
		if (free)
			__blk_put_request(q, free);
		else
			elv_merged_request(q, req, ELEVATOR_FRONT_MERGE);
		goto out_unlock;
	default:
		break;
	}

get_rq:
	rq_qos_throttle(q, bio, q->queue_lock);

	 
	blk_queue_enter_live(q);
	req = get_request(q, bio->bi_opf, bio, 0, GFP_NOIO);
	if (IS_ERR(req)) {
		blk_queue_exit(q);
		rq_qos_cleanup(q, bio);
		if (PTR_ERR(req) == -ENOMEM)
			bio->bi_status = BLK_STS_RESOURCE;
		else
			bio->bi_status = BLK_STS_IOERR;
		bio_endio(bio);
		goto out_unlock;
	}

	rq_qos_track(q, req, bio);

	 
	blk_init_request_from_bio(req, bio);

	if (test_bit(QUEUE_FLAG_SAME_COMP, &q->queue_flags))
		req->cpu = raw_smp_processor_id();

	plug = current->plug;
	if (plug) {
		 
		if (!request_count || list_empty(&plug->list))
			trace_block_plug(q);
		else {
			struct request *last = list_entry_rq(plug->list.prev);
			if (request_count >= BLK_MAX_REQUEST_COUNT ||
			    blk_rq_bytes(last) >= BLK_PLUG_FLUSH_SIZE) {
				blk_flush_plug_list(plug, false);
				trace_block_plug(q);
			}
		}
		list_add_tail(&req->queuelist, &plug->list);
		blk_account_io_start(req, true);
	} else {
		spin_lock_irq(q->queue_lock);
		add_acct_request(q, req, where);
		__blk_run_queue(q);
out_unlock:
		spin_unlock_irq(q->queue_lock);
	}

	return BLK_QC_T_NONE;
}
