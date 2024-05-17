vrrp_vscript_timeout_handler(vector_t *strvec)
{
	vrrp_script_t *vscript = LIST_TAIL_DATA(vrrp_data->vrrp_script);
	unsigned timeout;

	 
	if (!read_unsigned_strvec(strvec, 1, &timeout, 0, UINT_MAX / TIMER_HZ, true)) {
		report_config_error(CONFIG_GENERAL_ERROR, "(%s): vrrp script timeout '%s' invalid - ignoring", vscript->sname, FMT_STR_VSLOT(strvec, 1));
		return;
	}

	if (timeout == 0) {
		report_config_error(CONFIG_GENERAL_ERROR, "(%s): vrrp script timeout must be greater than 0, setting to 1", vscript->sname);
		timeout = 1;
	}

	vscript->timeout = timeout * TIMER_HZ;
}