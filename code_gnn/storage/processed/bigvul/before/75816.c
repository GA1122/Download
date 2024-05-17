default_interface_handler(vector_t *strvec)
{
	if (vector_size(strvec) < 2) {
		report_config_error(CONFIG_GENERAL_ERROR, "default_interface requires interface name");
		return;
	}
	FREE_PTR(global_data->default_ifname);
	global_data->default_ifname = set_value(strvec);

	 
#ifndef _DEBUG_
	if (prog_type == PROG_TYPE_VRRP)
#endif
	{
		global_data->default_ifp = if_get_by_ifname(global_data->default_ifname, IF_CREATE_IF_DYNAMIC);
		if (!global_data->default_ifp)
			report_config_error(CONFIG_GENERAL_ERROR, "WARNING - default interface %s doesn't exist", global_data->default_ifname);
	}
}