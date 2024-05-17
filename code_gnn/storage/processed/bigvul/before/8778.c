static NOINLINE int send_select(uint32_t xid, uint32_t server, uint32_t requested)
{
	struct dhcp_packet packet;
	struct in_addr temp_addr;

 
	 
	init_packet(&packet, DHCPREQUEST);

	packet.xid = xid;
	udhcp_add_simple_option(&packet, DHCP_REQUESTED_IP, requested);

	udhcp_add_simple_option(&packet, DHCP_SERVER_ID, server);

	 
	add_client_options(&packet);

	temp_addr.s_addr = requested;
	bb_error_msg("sending select for %s", inet_ntoa(temp_addr));
	return raw_bcast_from_client_config_ifindex(&packet, INADDR_ANY);
}
