static struct request *__get_request(struct request_list *rl, unsigned int op,
		struct bio *bio, blk_mq_req_flags_t flags, gfp_t gfp_mask)
{
	struct request_queue *q = rl->q;
	struct request *rq;
	struct elevator_type *et = q->elevator->type;
	struct io_context *ioc = rq_ioc(bio);
	struct io_cq *icq = NULL;
	const bool is_sync = op_is_sync(op);
	int may_queue;
	req_flags_t rq_flags = RQF_ALLOCED;

	lockdep_assert_held(q->queue_lock);

	if (unlikely(blk_queue_dying(q)))
		return ERR_PTR(-ENODEV);

	may_queue = elv_may_queue(q, op);
	if (may_queue == ELV_MQUEUE_NO)
		goto rq_starved;

	if (rl->count[is_sync]+1 >= queue_congestion_on_threshold(q)) {
		if (rl->count[is_sync]+1 >= q->nr_requests) {
			 
			if (!blk_rl_full(rl, is_sync)) {
				ioc_set_batching(q, ioc);
				blk_set_rl_full(rl, is_sync);
			} else {
				if (may_queue != ELV_MQUEUE_MUST
						&& !ioc_batching(q, ioc)) {
					 
					return ERR_PTR(-ENOMEM);
				}
			}
		}
		blk_set_congested(rl, is_sync);
	}

	 
	if (rl->count[is_sync] >= (3 * q->nr_requests / 2))
		return ERR_PTR(-ENOMEM);

	q->nr_rqs[is_sync]++;
	rl->count[is_sync]++;
	rl->starved[is_sync] = 0;

	 
	if (!op_is_flush(op) && !blk_queue_bypass(q)) {
		rq_flags |= RQF_ELVPRIV;
		q->nr_rqs_elvpriv++;
		if (et->icq_cache && ioc)
			icq = ioc_lookup_icq(ioc, q);
	}

	if (blk_queue_io_stat(q))
		rq_flags |= RQF_IO_STAT;
	spin_unlock_irq(q->queue_lock);

	 
	rq = mempool_alloc(rl->rq_pool, gfp_mask);
	if (!rq)
		goto fail_alloc;

	blk_rq_init(q, rq);
	blk_rq_set_rl(rq, rl);
	rq->cmd_flags = op;
	rq->rq_flags = rq_flags;
	if (flags & BLK_MQ_REQ_PREEMPT)
		rq->rq_flags |= RQF_PREEMPT;

	 
	if (rq_flags & RQF_ELVPRIV) {
		if (unlikely(et->icq_cache && !icq)) {
			if (ioc)
				icq = ioc_create_icq(ioc, q, gfp_mask);
			if (!icq)
				goto fail_elvpriv;
		}

		rq->elv.icq = icq;
		if (unlikely(elv_set_request(q, rq, bio, gfp_mask)))
			goto fail_elvpriv;

		 
		if (icq)
			get_io_context(icq->ioc);
	}
out:
	 
	if (ioc_batching(q, ioc))
		ioc->nr_batch_requests--;

	trace_block_getrq(q, bio, op);
	return rq;

fail_elvpriv:
	 
	printk_ratelimited(KERN_WARNING "%s: dev %s: request aux data allocation failed, iosched may be disturbed\n",
			   __func__, dev_name(q->backing_dev_info->dev));

	rq->rq_flags &= ~RQF_ELVPRIV;
	rq->elv.icq = NULL;

	spin_lock_irq(q->queue_lock);
	q->nr_rqs_elvpriv--;
	spin_unlock_irq(q->queue_lock);
	goto out;

fail_alloc:
	 
	spin_lock_irq(q->queue_lock);
	freed_request(rl, is_sync, rq_flags);

	 
rq_starved:
	if (unlikely(rl->count[is_sync] == 0))
		rl->starved[is_sync] = 1;
	return ERR_PTR(-ENOMEM);
}
