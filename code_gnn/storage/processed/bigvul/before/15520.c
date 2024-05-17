static NOINLINE int send_select(uint32_t xid, uint32_t server, uint32_t requested)
{
	struct dhcp_packet packet;
	struct in_addr addr;

 
	 
	init_packet(&packet, DHCPREQUEST);

	packet.xid = xid;
	udhcp_add_simple_option(&packet, DHCP_REQUESTED_IP, requested);

	udhcp_add_simple_option(&packet, DHCP_SERVER_ID, server);

	 
	add_client_options(&packet);

	addr.s_addr = requested;
	bb_info_msg("Sending select for %s...", inet_ntoa(addr));
	return raw_bcast_from_client_config_ifindex(&packet);
}
