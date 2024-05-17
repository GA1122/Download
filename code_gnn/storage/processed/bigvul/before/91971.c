static void blk_dequeue_request(struct request *rq)
{
	struct request_queue *q = rq->q;

	BUG_ON(list_empty(&rq->queuelist));
	BUG_ON(ELV_ON_HASH(rq));

	list_del_init(&rq->queuelist);

	 
	if (blk_account_rq(rq))
		q->in_flight[rq_is_sync(rq)]++;
}
