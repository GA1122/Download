smtpto_handler(vector_t *strvec)
{
	unsigned timeout;

	 
	if (!read_unsigned_strvec(strvec, 1, &timeout, 0, UINT_MAX / TIMER_HZ, true)) {
		report_config_error(CONFIG_GENERAL_ERROR, "smtp_connect_timeout '%s' must be in [0, %d] - ignoring", FMT_STR_VSLOT(strvec, 1), UINT_MAX / TIMER_HZ);
		return;
	}

	if (timeout == 0) {
		report_config_error(CONFIG_GENERAL_ERROR, "smtp_conect_timeout must be greater than 0, setting to 1");
		timeout = 1;
	}

	global_data->smtp_connection_to = timeout * TIMER_HZ;
}