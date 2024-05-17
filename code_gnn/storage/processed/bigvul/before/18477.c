static int kvp_set_ip_info(char *if_name, struct hv_kvp_ipaddr_value *new_val)
{
	int error = 0;
	char if_file[128];
	FILE *file;
	char cmd[512];
	char *mac_addr;

	 

	snprintf(if_file, sizeof(if_file), "%s%s%s", KVP_CONFIG_LOC,
		"hyperv/ifcfg-", if_name);

	file = fopen(if_file, "w");

	if (file == NULL) {
		syslog(LOG_ERR, "Failed to open config file");
		return HV_E_FAIL;
	}

	 

	mac_addr = kvp_if_name_to_mac(if_name);
	if (mac_addr == NULL) {
		error = HV_E_FAIL;
		goto setval_error;
	}

	error = kvp_write_file(file, "HWADDR", "", mac_addr);
	if (error)
		goto setval_error;

	error = kvp_write_file(file, "IF_NAME", "", if_name);
	if (error)
		goto setval_error;

	if (new_val->dhcp_enabled) {
		error = kvp_write_file(file, "DHCP", "", "yes");
		if (error)
			goto setval_error;

		 
		goto setval_done;
	}

	 

	error = process_ip_string(file, (char *)new_val->ip_addr, IPADDR);
	if (error)
		goto setval_error;

	error = process_ip_string(file, (char *)new_val->sub_net, NETMASK);
	if (error)
		goto setval_error;

	error = process_ip_string(file, (char *)new_val->gate_way, GATEWAY);
	if (error)
		goto setval_error;

	error = process_ip_string(file, (char *)new_val->dns_addr, DNS);
	if (error)
		goto setval_error;

setval_done:
	free(mac_addr);
	fclose(file);

	 

	snprintf(cmd, sizeof(cmd), "%s %s", "hv_set_ifconfig", if_file);
	system(cmd);
	return 0;

setval_error:
	syslog(LOG_ERR, "Failed to write config file");
	free(mac_addr);
	fclose(file);
	return error;
}
