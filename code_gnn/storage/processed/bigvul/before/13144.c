static void send_packet_to_relay(struct dhcp_packet *dhcp_pkt)
{
	log1("forwarding packet to relay");

	udhcp_send_kernel_packet(dhcp_pkt,
			server_config.server_nip, SERVER_PORT,
			dhcp_pkt->gateway_nip, SERVER_PORT);
}
