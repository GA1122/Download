void balloon_set_new_target(unsigned long target)
{
	 
	balloon_stats.target_pages = target;
	schedule_delayed_work(&balloon_worker, 0);
}
