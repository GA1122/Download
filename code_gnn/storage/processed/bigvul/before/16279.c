int SafeSock::get_bytes(void *dta, int size)
{
	ASSERT( size > 0 );
	while(!_msgReady) {
		if(_timeout > 0) {
			Selector selector;
			selector.set_timeout( _timeout );
			selector.add_fd( _sock, Selector::IO_READ );
				
			selector.execute();
			if ( selector.timed_out() ) {
				return 0;
			} else if ( !selector.has_ready() ) {
					dprintf(D_NETWORK, "select returns %d, recv failed\n",
							selector.select_retval());
					return 0;
			}
		}
		(void)handle_incoming_packet();
	}

	char *tempBuf = (char *)malloc(size);
    if (!tempBuf) { EXCEPT("malloc failed"); }
	int readSize, length;
    unsigned char * dec;

	if(_longMsg) {
        readSize = _longMsg->getn(tempBuf, size);
    }
	else { 
        readSize = _shortMsg.getn(tempBuf, size);
    }


	if(readSize == size) {
            if (get_encryption()) {
                unwrap((unsigned char *) tempBuf, readSize, dec, length);
                memcpy(dta, dec, readSize);
                free(dec);
            }
            else {
                memcpy(dta, tempBuf, readSize);
            }


            free(tempBuf);
            return readSize;
	} else {
		free(tempBuf);
        dprintf(D_NETWORK,
                "SafeSock::get_bytes - failed because bytes read is different from bytes requested\n");
		return -1;
	}
}
