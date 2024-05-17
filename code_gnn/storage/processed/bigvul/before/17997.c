ssh_remote_ipaddr(struct ssh *ssh)
{
	const int sock = ssh->state->connection_in;

	 
	if (ssh->remote_ipaddr == NULL) {
		if (ssh_packet_connection_is_on_socket(ssh)) {
			ssh->remote_ipaddr = get_peer_ipaddr(sock);
			ssh->remote_port = get_peer_port(sock);
			ssh->local_ipaddr = get_local_ipaddr(sock);
			ssh->local_port = get_local_port(sock);
		} else {
			ssh->remote_ipaddr = strdup("UNKNOWN");
			ssh->remote_port = 65535;
			ssh->local_ipaddr = strdup("UNKNOWN");
			ssh->local_port = 65535;
		}
	}
	return ssh->remote_ipaddr;
}
