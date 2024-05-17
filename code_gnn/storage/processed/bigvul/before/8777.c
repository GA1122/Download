static NOINLINE int send_renew(uint32_t xid, uint32_t server, uint32_t ciaddr)
{
	struct dhcp_packet packet;
	struct in_addr temp_addr;

 
	 
	init_packet(&packet, DHCPREQUEST);

	packet.xid = xid;
	packet.ciaddr = ciaddr;

	 
	add_client_options(&packet);

	temp_addr.s_addr = server;
	bb_error_msg("sending renew to %s", inet_ntoa(temp_addr));
	return bcast_or_ucast(&packet, ciaddr, server);
}
