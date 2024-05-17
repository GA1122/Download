static int send_release(uint32_t server, uint32_t ciaddr)
{
	struct dhcp_packet packet;

	 
	init_packet(&packet, DHCPRELEASE);

	 
	packet.ciaddr = ciaddr;

	udhcp_add_simple_option(&packet, DHCP_SERVER_ID, server);

	bb_info_msg("Sending release...");
	 
	return bcast_or_ucast(&packet, ciaddr, server);
}
