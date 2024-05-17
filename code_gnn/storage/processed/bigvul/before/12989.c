ssh_packet_set_nonblocking(struct ssh *ssh)
{
	 
	set_nonblock(ssh->state->connection_in);

	if (ssh->state->connection_out != ssh->state->connection_in)
		set_nonblock(ssh->state->connection_out);
}
