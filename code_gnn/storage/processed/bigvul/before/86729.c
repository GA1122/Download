static void blk_mq_queue_reinit(struct request_queue *q)
{
	WARN_ON_ONCE(!atomic_read(&q->mq_freeze_depth));

	blk_mq_sysfs_unregister(q);

	blk_mq_update_queue_map(q->mq_map, q->nr_hw_queues);

	 

	blk_mq_map_swqueue(q);

	blk_mq_sysfs_register(q);
}
