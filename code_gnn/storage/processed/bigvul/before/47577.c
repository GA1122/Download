void ap_bus_force_rescan(void)
{
	 
	mod_timer(&ap_config_timer, jiffies + ap_config_time * HZ);
	 
	queue_work(ap_work_queue, &ap_config_work);
	flush_work(&ap_config_work);
}
