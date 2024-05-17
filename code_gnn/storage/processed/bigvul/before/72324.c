process_output(fd_set *writeset, int connection_out)
{
	 
	if (FD_ISSET(connection_out, writeset))
		packet_write_poll();
}
