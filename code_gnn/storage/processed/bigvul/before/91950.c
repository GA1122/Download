inline void __blk_run_queue_uncond(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	if (unlikely(blk_queue_dead(q)))
		return;

	 
	q->request_fn_active++;
	q->request_fn(q);
	q->request_fn_active--;
}
