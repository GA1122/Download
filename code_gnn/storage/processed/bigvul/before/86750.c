static void blk_mq_usage_counter_release(struct percpu_ref *ref)
{
	struct request_queue *q =
		container_of(ref, struct request_queue, mq_usage_counter);

	wake_up_all(&q->mq_freeze_wq);
}