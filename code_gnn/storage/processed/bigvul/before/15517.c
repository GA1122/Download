static NOINLINE int send_discover(uint32_t xid, uint32_t requested)
{
	struct dhcp_packet packet;

	 
	init_packet(&packet, DHCPDISCOVER);

	packet.xid = xid;
	if (requested)
		udhcp_add_simple_option(&packet, DHCP_REQUESTED_IP, requested);

	 
	add_client_options(&packet);

	bb_info_msg("Sending discover...");
	return raw_bcast_from_client_config_ifindex(&packet);
}
