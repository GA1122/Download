int SafeSock::connect(
	char const	*host,
	int		port, 
	bool
	)
{
	if (!host || port < 0) return FALSE;

	_who.clear();
	if (!Sock::guess_address_string(host, port, _who))
		return FALSE;

	if (host[0] == '<') {
		set_connect_addr(host);
		} else {
		set_connect_addr(_who.to_sinful().Value());
	}
    addr_changed();

	int retval=special_connect(host,port,true);
	if( retval != CEDAR_ENOCCB ) {
		return retval;
	}

	 
	 
	 
	if (_state == sock_virgin || _state == sock_assigned) {
		bind(true);
	}

	if (_state != sock_bound) {
		dprintf(D_ALWAYS,
		        "SafeSock::connect bind() failed: _state = %d\n",
			  _state); 
		return FALSE;
	}
	
	_state = sock_connect;
	return TRUE;
}
