lvs_timeouts(vector_t *strvec)
{
	unsigned val;
	size_t i;

	if (vector_size(strvec) < 3) {
		report_config_error(CONFIG_GENERAL_ERROR, "lvs_timeouts requires at least one option");
		return;
	}

	for (i = 1; i < vector_size(strvec); i++) {
		if (!strcmp(strvec_slot(strvec, i), "tcp")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_timeout tcp - ignoring");
				continue;
			}
			if (!read_unsigned_strvec(strvec, i + 1, &val, 0, LVS_MAX_TIMEOUT, false))
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid lvs_timeout tcp (%s) - ignoring", FMT_STR_VSLOT(strvec, i+1));
			else
				global_data->lvs_tcp_timeout = val;
			i++;	 
			continue;
		}
		if (!strcmp(strvec_slot(strvec, i), "tcpfin")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_timeout tcpfin - ignoring");
				continue;
			}
			if (!read_unsigned_strvec(strvec, i + 1, &val, 0, LVS_MAX_TIMEOUT, false))
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid lvs_timeout tcpfin (%s) - ignoring", FMT_STR_VSLOT(strvec, i+1));
			else
				global_data->lvs_tcpfin_timeout = val;
			i++;	 
			continue;
		}
		if (!strcmp(strvec_slot(strvec, i), "udp")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_timeout udp - ignoring");
				continue;
			}
			if (!read_unsigned_strvec(strvec, i + 1, &val, 0, LVS_MAX_TIMEOUT, false))
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid lvs_timeout udp (%s) - ignoring", FMT_STR_VSLOT(strvec, i+1));
			else
				global_data->lvs_udp_timeout = val;
			i++;	 
			continue;
		}
		report_config_error(CONFIG_GENERAL_ERROR, "Unknown option %s specified for lvs_timeouts", FMT_STR_VSLOT(strvec, i));
	}
}