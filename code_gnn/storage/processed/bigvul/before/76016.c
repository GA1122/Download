vrrp_lvs_syncd_handler(vector_t *strvec)
{
	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);

	report_config_error(CONFIG_GENERAL_ERROR, "(%s) Specifying lvs_sync_daemon_interface against a vrrp is deprecated.", vrrp->iname);   
	report_config_error(CONFIG_GENERAL_ERROR, "      %*sPlease use global lvs_sync_daemon", (int)strlen(vrrp->iname) - 2, "");

	if (global_data->lvs_syncd.ifname) {
		report_config_error(CONFIG_GENERAL_ERROR, "(%s) lvs_sync_daemon_interface has already been specified as %s - ignoring", vrrp->iname, global_data->lvs_syncd.ifname);
		return;
	}

	global_data->lvs_syncd.ifname = set_value(strvec);
	global_data->lvs_syncd.vrrp = vrrp;
}