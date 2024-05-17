int SafeSock::get_ptr(void *&ptr, char delim)
{
	int size;

	while(!_msgReady) {
		if(_timeout > 0) {
			Selector selector;
			selector.set_timeout( _timeout );
			selector.add_fd( _sock, Selector::IO_READ );
				
			selector.execute();
			if ( selector.timed_out() ) {
				return 0;
			} else if ( !selector.has_ready() ) {
					dprintf(D_NETWORK,
					        "select returns %d, recv failed\n",
						  selector.select_retval() );
					return 0;
			}
		}
		(void)handle_incoming_packet();
	}

	if(_longMsg) {  
		size = _longMsg->getPtr(ptr, delim);
		return size;
	}
	else {  
		size = _shortMsg.getPtr(ptr, delim);
		return size;
	}
}
