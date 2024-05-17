static int blk_mq_queue_reinit_notify(struct notifier_block *nb,
				      unsigned long action, void *hcpu)
{
	struct request_queue *q;

	 
	if (action != CPU_DEAD && action != CPU_DEAD_FROZEN &&
	    action != CPU_ONLINE && action != CPU_ONLINE_FROZEN)
		return NOTIFY_OK;

	mutex_lock(&all_q_mutex);

	 
	list_for_each_entry(q, &all_q_list, all_q_node)
		blk_mq_freeze_queue_start(q);
	list_for_each_entry(q, &all_q_list, all_q_node) {
		blk_mq_freeze_queue_wait(q);

		 
		del_timer_sync(&q->timeout);
	}

	list_for_each_entry(q, &all_q_list, all_q_node)
		blk_mq_queue_reinit(q);

	list_for_each_entry(q, &all_q_list, all_q_node)
		blk_mq_unfreeze_queue(q);

	mutex_unlock(&all_q_mutex);
	return NOTIFY_OK;
}
