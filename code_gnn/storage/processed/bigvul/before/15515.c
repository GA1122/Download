static int raw_bcast_from_client_config_ifindex(struct dhcp_packet *packet)
{
	return udhcp_send_raw_packet(packet,
		  INADDR_ANY, CLIENT_PORT,
		  INADDR_BROADCAST, SERVER_PORT, MAC_BCAST_ADDR,
		client_config.ifindex);
}