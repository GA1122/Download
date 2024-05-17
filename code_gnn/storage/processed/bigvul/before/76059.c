vrrp_vscript_interval_handler(vector_t *strvec)
{
	vrrp_script_t *vscript = LIST_TAIL_DATA(vrrp_data->vrrp_script);
	unsigned interval;

	 
	if (!read_unsigned_strvec(strvec, 1, &interval, 0, UINT_MAX / TIMER_HZ, true)) {
		report_config_error(CONFIG_GENERAL_ERROR, "(%s): vrrp script interval '%s' must be between 1 and %u - ignoring", vscript->sname, FMT_STR_VSLOT(strvec, 1), UINT_MAX / TIMER_HZ);
		return;
	}

	if (interval == 0) {
		report_config_error(CONFIG_GENERAL_ERROR, "(%s): vrrp script interval must be greater than 0, setting to 1", vscript->sname);
		interval = 1;
	}

	vscript->interval = interval * TIMER_HZ;
}
