void blkcg_drain_queue(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);

	 
	if (!q->root_blkg)
		return;

	blk_throtl_drain(q);
}
