static NOINLINE int send_renew(uint32_t xid, uint32_t server, uint32_t ciaddr)
{
	struct dhcp_packet packet;

 
	 
	init_packet(&packet, DHCPREQUEST);

	packet.xid = xid;
	packet.ciaddr = ciaddr;

	 
	add_client_options(&packet);

	bb_info_msg("Sending renew...");
	return bcast_or_ucast(&packet, ciaddr, server);
}
