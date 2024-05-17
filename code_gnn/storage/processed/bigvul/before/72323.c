process_input(fd_set *readset, int connection_in)
{
	struct ssh *ssh = active_state;  
	int len;
	char buf[16384];

	 
	if (FD_ISSET(connection_in, readset)) {
		len = read(connection_in, buf, sizeof(buf));
		if (len == 0) {
			verbose("Connection closed by %.100s port %d",
			    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
			return -1;
		} else if (len < 0) {
			if (errno != EINTR && errno != EAGAIN) {
				verbose("Read error from remote host "
				    "%.100s port %d: %.100s",
				    ssh_remote_ipaddr(ssh),
				    ssh_remote_port(ssh), strerror(errno));
				cleanup_exit(255);
			}
		} else {
			 
			packet_process_incoming(buf, len);
		}
	}
	return 0;
}
