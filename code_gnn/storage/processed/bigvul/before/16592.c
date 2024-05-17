FileTransfer::ReceiveTransferGoAhead(
	Stream *s,
	char const *fname,
	bool downloading,
	bool &go_ahead_always)
{
	bool try_again = true;
	int hold_code = 0;
	int hold_subcode = 0;
	MyString error_desc;
	bool result;
	int alive_interval;
	int old_timeout;
	const int slop_time = 20;  
	const int min_alive_interval = 300;


	alive_interval = clientSockTimeout;
	if( alive_interval < min_alive_interval ) {
		alive_interval = min_alive_interval;
	}
	old_timeout = s->timeout(alive_interval + slop_time);

	result = DoReceiveTransferGoAhead(s,fname,downloading,go_ahead_always,try_again,hold_code,hold_subcode,error_desc,alive_interval);

	s->timeout( old_timeout );

	if( !result ) {
		SaveTransferInfo(false,try_again,hold_code,hold_subcode,error_desc.Value());
		if( error_desc.Length() ) {
			dprintf(D_ALWAYS,"%s\n",error_desc.Value());
		}
	}

	return result;
}
