lvs_syncd_handler(vector_t *strvec)
{
	unsigned val;
	size_t i;

	if (global_data->lvs_syncd.ifname) {
		report_config_error(CONFIG_GENERAL_ERROR, "lvs_sync_daemon has already been specified as %s %s - ignoring", global_data->lvs_syncd.ifname, global_data->lvs_syncd.vrrp_name);
		return;
	}

	if (vector_size(strvec) < 3) {
		report_config_error(CONFIG_GENERAL_ERROR, "lvs_sync_daemon requires interface, VRRP instance");
		return;
	}

	if (strlen(strvec_slot(strvec, 1)) >= IP_VS_IFNAME_MAXLEN) {
		report_config_error(CONFIG_GENERAL_ERROR, "lvs_sync_daemon interface name '%s' too long - ignoring", FMT_STR_VSLOT(strvec, 1));
		return;
	}

	if (strlen(strvec_slot(strvec, 2)) >= IP_VS_IFNAME_MAXLEN) {
		report_config_error(CONFIG_GENERAL_ERROR, "lvs_sync_daemon vrrp interface name '%s' too long - ignoring", FMT_STR_VSLOT(strvec, 2));
		return;
	}

	global_data->lvs_syncd.ifname = set_value(strvec);

	global_data->lvs_syncd.vrrp_name = MALLOC(strlen(strvec_slot(strvec, 2)) + 1);
	if (!global_data->lvs_syncd.vrrp_name)
		return;
	strcpy(global_data->lvs_syncd.vrrp_name, strvec_slot(strvec, 2));

	 
	if (vector_size(strvec) >= 4 && isdigit(FMT_STR_VSLOT(strvec, 3)[0])) {
		report_config_error(CONFIG_GENERAL_ERROR, "Please use keyword \"id\" before lvs_sync_daemon syncid value");
		if (!read_unsigned_strvec(strvec, 3, &val, 0, 255, false))
			report_config_error(CONFIG_GENERAL_ERROR, "Invalid syncid (%s) - defaulting to vrid", FMT_STR_VSLOT(strvec, 3));
		else
			global_data->lvs_syncd.syncid = val;
		i = 4;
	}
	else
		i = 3;

	for ( ; i < vector_size(strvec); i++) {
		if (!strcmp(strvec_slot(strvec, i), "id")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_sync_daemon id, defaulting to vrid");
				continue;
			}
			if (!read_unsigned_strvec(strvec, i + 1, &val, 0, 255, false))
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid syncid (%s) - defaulting to vrid", FMT_STR_VSLOT(strvec, i+1));
			else
				global_data->lvs_syncd.syncid = val;
			i++;	 
			continue;
		}
#ifdef _HAVE_IPVS_SYNCD_ATTRIBUTES_
		if (!strcmp(strvec_slot(strvec, i), "maxlen")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_sync_daemon maxlen - ignoring");
				continue;
			}
			if (!read_unsigned_strvec(strvec, i + 1, &val, 0, 65535 - 20 - 8, false))
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid lvs_sync_daemon maxlen (%s) - ignoring", FMT_STR_VSLOT(strvec, i+1));
			else
				global_data->lvs_syncd.sync_maxlen = (uint16_t)val;
			i++;	 
			continue;
		}
		if (!strcmp(strvec_slot(strvec, i), "port")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_sync_daemon port - ignoring");
				continue;
			}
			if (!read_unsigned_strvec(strvec, i + 1, &val, 0, 65535, false))
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid lvs_sync_daemon port (%s) - ignoring", FMT_STR_VSLOT(strvec, i+1));
			else
				global_data->lvs_syncd.mcast_port = (uint16_t)val;
			i++;	 
			continue;
		}
		if (!strcmp(strvec_slot(strvec, i), "ttl")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_sync_daemon ttl - ignoring");
				continue;
			}
			if (!read_unsigned_strvec(strvec, i + 1, &val, 0, 255, false))
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid lvs_sync_daemon ttl (%s) - ignoring", FMT_STR_VSLOT(strvec, i+1));
			else
				global_data->lvs_syncd.mcast_ttl = (uint8_t)val;
			i++;	 
			continue;
		}
		if (!strcmp(strvec_slot(strvec, i), "group")) {
			if (i == vector_size(strvec) - 1) {
				report_config_error(CONFIG_GENERAL_ERROR, "No value specified for lvs_sync_daemon group - ignoring");
				continue;
			}

			if (inet_stosockaddr(strvec_slot(strvec, i+1), NULL, &global_data->lvs_syncd.mcast_group) < 0)
				report_config_error(CONFIG_GENERAL_ERROR, "Invalid lvs_sync_daemon group (%s) - ignoring", FMT_STR_VSLOT(strvec, i+1));

			if ((global_data->lvs_syncd.mcast_group.ss_family == AF_INET  && !IN_MULTICAST(htonl(((struct sockaddr_in *)&global_data->lvs_syncd.mcast_group)->sin_addr.s_addr))) ||
			    (global_data->lvs_syncd.mcast_group.ss_family == AF_INET6 && !IN6_IS_ADDR_MULTICAST(&((struct sockaddr_in6 *)&global_data->lvs_syncd.mcast_group)->sin6_addr))) {
				report_config_error(CONFIG_GENERAL_ERROR, "lvs_sync_daemon group address %s is not multicast - ignoring", FMT_STR_VSLOT(strvec, i+1));
				global_data->lvs_syncd.mcast_group.ss_family = AF_UNSPEC;
			}

			i++;	 
			continue;
		}
#endif
		report_config_error(CONFIG_GENERAL_ERROR, "Unknown option %s specified for lvs_sync_daemon", FMT_STR_VSLOT(strvec, i));
	}
}