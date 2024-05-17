vrrp_ipsets_handler(vector_t *strvec)
{
	size_t len;

	if (vector_size(strvec) >= 2) {
		if (strlen(strvec_slot(strvec,1)) >= sizeof(global_data->vrrp_ipset_address)-1) {
			report_config_error(CONFIG_GENERAL_ERROR, "VRRP Error : ipset address name too long - ignored");
			return;
		}
		strcpy(global_data->vrrp_ipset_address, strvec_slot(strvec,1));
	}
	else {
		global_data->using_ipsets = false;
		return;
	}

	if (vector_size(strvec) >= 3) {
		if (strlen(strvec_slot(strvec,2)) >= sizeof(global_data->vrrp_ipset_address6)-1) {
			report_config_error(CONFIG_GENERAL_ERROR, "VRRP Error : ipset IPv6 address name too long - ignored");
			return;
		}
		strcpy(global_data->vrrp_ipset_address6, strvec_slot(strvec,2));
	}
	else {
		 
		strcpy(global_data->vrrp_ipset_address6, global_data->vrrp_ipset_address);
		global_data->vrrp_ipset_address6[sizeof(global_data->vrrp_ipset_address6) - 2] = '\0';
		strcat(global_data->vrrp_ipset_address6, "6");
	}
	if (vector_size(strvec) >= 4) {
		if (strlen(strvec_slot(strvec,3)) >= sizeof(global_data->vrrp_ipset_address_iface6)-1) {
			report_config_error(CONFIG_GENERAL_ERROR, "VRRP Error : ipset IPv6 address_iface name too long - ignored");
			return;
		}
		strcpy(global_data->vrrp_ipset_address_iface6, strvec_slot(strvec,3));
	}
	else {
		 
		strcpy(global_data->vrrp_ipset_address_iface6, global_data->vrrp_ipset_address6);
		len = strlen(global_data->vrrp_ipset_address_iface6);
		if (global_data->vrrp_ipset_address_iface6[len-1] == '6')
			global_data->vrrp_ipset_address_iface6[--len] = '\0';
		global_data->vrrp_ipset_address_iface6[sizeof(global_data->vrrp_ipset_address_iface6) - 5] = '\0';
		strcat(global_data->vrrp_ipset_address_iface6, "_if6");
	}
}