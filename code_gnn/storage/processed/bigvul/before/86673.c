static void __blk_mq_run_hw_queue(struct blk_mq_hw_ctx *hctx)
{
	struct request_queue *q = hctx->queue;
	struct request *rq;
	LIST_HEAD(rq_list);
	LIST_HEAD(driver_list);
	struct list_head *dptr;
	int queued;

	WARN_ON(!cpumask_test_cpu(raw_smp_processor_id(), hctx->cpumask));

	if (unlikely(test_bit(BLK_MQ_S_STOPPED, &hctx->state)))
		return;

	hctx->run++;

	 
	flush_busy_ctxs(hctx, &rq_list);

	 
	if (!list_empty_careful(&hctx->dispatch)) {
		spin_lock(&hctx->lock);
		if (!list_empty(&hctx->dispatch))
			list_splice_init(&hctx->dispatch, &rq_list);
		spin_unlock(&hctx->lock);
	}

	 
	dptr = NULL;

	 
	queued = 0;
	while (!list_empty(&rq_list)) {
		struct blk_mq_queue_data bd;
		int ret;

		rq = list_first_entry(&rq_list, struct request, queuelist);
		list_del_init(&rq->queuelist);

		bd.rq = rq;
		bd.list = dptr;
		bd.last = list_empty(&rq_list);

		ret = q->mq_ops->queue_rq(hctx, &bd);
		switch (ret) {
		case BLK_MQ_RQ_QUEUE_OK:
			queued++;
			continue;
		case BLK_MQ_RQ_QUEUE_BUSY:
			list_add(&rq->queuelist, &rq_list);
			__blk_mq_requeue_request(rq);
			break;
		default:
			pr_err("blk-mq: bad return on queue: %d\n", ret);
		case BLK_MQ_RQ_QUEUE_ERROR:
			rq->errors = -EIO;
			blk_mq_end_request(rq, rq->errors);
			break;
		}

		if (ret == BLK_MQ_RQ_QUEUE_BUSY)
			break;

		 
		if (!dptr && rq_list.next != rq_list.prev)
			dptr = &driver_list;
	}

	if (!queued)
		hctx->dispatched[0]++;
	else if (queued < (1 << (BLK_MQ_MAX_DISPATCH_ORDER - 1)))
		hctx->dispatched[ilog2(queued) + 1]++;

	 
	if (!list_empty(&rq_list)) {
		spin_lock(&hctx->lock);
		list_splice(&rq_list, &hctx->dispatch);
		spin_unlock(&hctx->lock);
		 
		blk_mq_run_hw_queue(hctx, true);
	}
}