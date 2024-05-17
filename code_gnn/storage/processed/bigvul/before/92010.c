void blk_queue_bypass_start(struct request_queue *q)
{
	WARN_ON_ONCE(q->mq_ops);

	spin_lock_irq(q->queue_lock);
	q->bypass_depth++;
	queue_flag_set(QUEUE_FLAG_BYPASS, q);
	spin_unlock_irq(q->queue_lock);

	 
	if (blk_queue_init_done(q)) {
		spin_lock_irq(q->queue_lock);
		__blk_drain_queue(q, false);
		spin_unlock_irq(q->queue_lock);

		 
		synchronize_rcu();
	}
}
