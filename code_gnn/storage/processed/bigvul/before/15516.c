static NOINLINE int send_decline(  uint32_t server, uint32_t requested)
{
	struct dhcp_packet packet;

	 
	init_packet(&packet, DHCPDECLINE);

#if 0
	 
	packet.xid = xid;
#endif
	 
	udhcp_add_simple_option(&packet, DHCP_REQUESTED_IP, requested);

	udhcp_add_simple_option(&packet, DHCP_SERVER_ID, server);

	bb_info_msg("Sending decline...");
	return raw_bcast_from_client_config_ifindex(&packet);
}
