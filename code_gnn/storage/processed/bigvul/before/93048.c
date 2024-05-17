rdpdr_check_fds(fd_set * rfds, fd_set * wfds, RD_BOOL timed_out)
{
	fd_set dummy;


	FD_ZERO(&dummy);


	 

	_rdpdr_check_fds(&dummy, &dummy, False);
	_rdpdr_check_fds(rfds, wfds, timed_out);
}
