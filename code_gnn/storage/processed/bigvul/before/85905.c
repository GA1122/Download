void dm_init_normal_md_queue(struct mapped_device *md)
{
	md->use_blk_mq = false;
	dm_init_md_queue(md);

	 
	md->queue->backing_dev_info->congested_fn = dm_any_congested;
}