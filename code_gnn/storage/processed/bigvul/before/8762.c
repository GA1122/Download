static void add_client_options(struct dhcp_packet *packet)
{
	int i, end, len;

	udhcp_add_simple_option(packet, DHCP_MAX_SIZE, htons(IP_UDP_DHCP_SIZE));

	 
	end = udhcp_end_option(packet->options);
	len = 0;
	for (i = 1; i < DHCP_END; i++) {
		if (client_config.opt_mask[i >> 3] & (1 << (i & 7))) {
			packet->options[end + OPT_DATA + len] = i;
			len++;
		}
	}
	if (len) {
		packet->options[end + OPT_CODE] = DHCP_PARAM_REQ;
		packet->options[end + OPT_LEN] = len;
		packet->options[end + OPT_DATA + len] = DHCP_END;
	}

	if (client_config.vendorclass)
		udhcp_add_binary_option(packet, client_config.vendorclass);
	if (client_config.hostname)
		udhcp_add_binary_option(packet, client_config.hostname);
	if (client_config.fqdn)
		udhcp_add_binary_option(packet, client_config.fqdn);

	 
	if ((option_mask32 & OPT_B) && packet->ciaddr == 0)
		packet->flags |= htons(BROADCAST_FLAG);

	 
	{
		struct option_set *curr = client_config.options;
		while (curr) {
			udhcp_add_binary_option(packet, curr->data);
			curr = curr->next;
		}
	}

}
