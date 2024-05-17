GahpServer::new_reqid()
{
	int starting_reqid;
	GahpClient* unused;

	starting_reqid  = next_reqid;
	
	next_reqid++;
	while (starting_reqid != next_reqid) {
		if ( next_reqid > 990000000 ) {
			next_reqid = 1;
			rotated_reqids = true;
		}
		if ( (!rotated_reqids) || 
			 (requestTable->lookup(next_reqid,unused) == -1) ) {
			return next_reqid;
		}
		next_reqid++;
	}

	EXCEPT("GAHP client - out of request ids !!!?!?!?");
	
	return -1;   
}
