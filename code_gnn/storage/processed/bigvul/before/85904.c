void dm_init_md_queue(struct mapped_device *md)
{
	 
	queue_flag_clear_unlocked(QUEUE_FLAG_STACKABLE, md->queue);

	 
	md->queue->queuedata = md;
	md->queue->backing_dev_info->congested_data = md;
}
