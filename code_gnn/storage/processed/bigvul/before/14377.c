child_close_fds(void)
{
	extern int auth_sock;

	if (auth_sock != -1) {
		close(auth_sock);
		auth_sock = -1;
	}

	if (packet_get_connection_in() == packet_get_connection_out())
		close(packet_get_connection_in());
	else {
		close(packet_get_connection_in());
		close(packet_get_connection_out());
	}
	 
	 
	channel_close_all();

	 
	endpwent();

	 
	closefrom(STDERR_FILENO + 1);
}
