int blk_queue_enter(struct request_queue *q, blk_mq_req_flags_t flags)
{
	const bool preempt = flags & BLK_MQ_REQ_PREEMPT;

	while (true) {
		bool success = false;

		rcu_read_lock();
		if (percpu_ref_tryget_live(&q->q_usage_counter)) {
			 
			if (preempt || !blk_queue_preempt_only(q)) {
				success = true;
			} else {
				percpu_ref_put(&q->q_usage_counter);
			}
		}
		rcu_read_unlock();

		if (success)
			return 0;

		if (flags & BLK_MQ_REQ_NOWAIT)
			return -EBUSY;

		 
		smp_rmb();

		wait_event(q->mq_freeze_wq,
			   (atomic_read(&q->mq_freeze_depth) == 0 &&
			    (preempt || !blk_queue_preempt_only(q))) ||
			   blk_queue_dying(q));
		if (blk_queue_dying(q))
			return -ENODEV;
	}
}
