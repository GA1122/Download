void disable_write_zeroes(struct mapped_device *md)
{
	struct queue_limits *limits = dm_get_queue_limits(md);

	 
	limits->max_write_zeroes_sectors = 0;
}
