net_namespace_handler(vector_t *strvec)
{
	if (!strvec)
		return;

	 
	if (!reload) {
		if (!global_data->network_namespace) {
			global_data->network_namespace = set_value(strvec);
			use_pid_dir = true;
		}
		else
			report_config_error(CONFIG_GENERAL_ERROR, "Duplicate net_namespace definition %s - ignoring", FMT_STR_VSLOT(strvec, 1));
	}
}
