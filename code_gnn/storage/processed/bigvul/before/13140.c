static NOINLINE void send_inform(struct dhcp_packet *oldpacket)
{
	struct dhcp_packet packet;

	 
	init_packet(&packet, oldpacket, DHCPACK);
	add_server_options(&packet);

	send_packet(&packet,   0);
}
