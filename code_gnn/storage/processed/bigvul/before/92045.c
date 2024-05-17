static struct request *elv_next_request(struct request_queue *q)
{
	struct request *rq;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, NULL);

	WARN_ON_ONCE(q->mq_ops);

	while (1) {
		list_for_each_entry(rq, &q->queue_head, queuelist) {
			if (blk_pm_allow_request(rq))
				return rq;

			if (rq->rq_flags & RQF_SOFTBARRIER)
				break;
		}

		 
		if (fq->flush_pending_idx != fq->flush_running_idx &&
				!queue_flush_queueable(q)) {
			fq->flush_queue_delayed = 1;
			return NULL;
		}
		if (unlikely(blk_queue_bypass(q)) ||
		    !q->elevator->type->ops.sq.elevator_dispatch_fn(q, 0))
			return NULL;
	}
}
