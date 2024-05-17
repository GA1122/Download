void blk_cleanup_queue(struct request_queue *q)
{
	spinlock_t *lock = q->queue_lock;

	 
	mutex_lock(&q->sysfs_lock);
	blk_set_queue_dying(q);
	spin_lock_irq(lock);

	 
	q->bypass_depth++;
	queue_flag_set(QUEUE_FLAG_BYPASS, q);

	queue_flag_set(QUEUE_FLAG_NOMERGES, q);
	queue_flag_set(QUEUE_FLAG_NOXMERGES, q);
	queue_flag_set(QUEUE_FLAG_DYING, q);
	spin_unlock_irq(lock);
	mutex_unlock(&q->sysfs_lock);

	 
	blk_freeze_queue(q);
	spin_lock_irq(lock);
	queue_flag_set(QUEUE_FLAG_DEAD, q);
	spin_unlock_irq(lock);

	 
	if (q->mq_ops && blk_queue_init_done(q))
		blk_mq_quiesce_queue(q);

	 
	blk_flush_integrity();

	 
	del_timer_sync(&q->backing_dev_info->laptop_mode_wb_timer);
	blk_sync_queue(q);

	 
	WARN_ON_ONCE(q->kobj.state_in_sysfs);

	 
	if (q->elevator) {
		ioc_clear_queue(q);
		elevator_exit(q, q->elevator);
		q->elevator = NULL;
	}

	 
	blkcg_exit_queue(q);

	 
	bdi_put(q->backing_dev_info);

	if (q->mq_ops)
		blk_mq_free_queue(q);
	percpu_ref_exit(&q->q_usage_counter);

	spin_lock_irq(lock);
	if (q->queue_lock != &q->__queue_lock)
		q->queue_lock = &q->__queue_lock;
	spin_unlock_irq(lock);

	 
	blk_put_queue(q);
}
