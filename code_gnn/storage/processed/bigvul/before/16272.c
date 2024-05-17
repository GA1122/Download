int SafeSock::attach_to_file_desc(int fd)
{
	if (_state != sock_virgin) return FALSE;

	_sock = fd;
	_state = sock_connect;
	timeout(0);  
	return TRUE;
}
