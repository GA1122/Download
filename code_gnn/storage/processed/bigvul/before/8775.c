static NOINLINE int send_discover(uint32_t xid, uint32_t requested)
{
	struct dhcp_packet packet;

	 
	init_packet(&packet, DHCPDISCOVER);

	packet.xid = xid;
	if (requested)
		udhcp_add_simple_option(&packet, DHCP_REQUESTED_IP, requested);

	 
	add_client_options(&packet);

	bb_error_msg("sending %s", "discover");
	return raw_bcast_from_client_config_ifindex(&packet, INADDR_ANY);
}
