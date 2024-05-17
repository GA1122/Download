static bool housekeeping_fn(const struct timeval *now, void *private_data)
{
	change_to_root_user();

	 
	update_monitored_printq_cache();

	 
	check_reload(time(NULL));

	 
	attempt_machine_password_change();

         
	force_check_log_size();
	check_log_size();
	return true;
}
