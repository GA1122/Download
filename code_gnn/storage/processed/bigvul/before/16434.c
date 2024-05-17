CStarter::remoteHoldCommand( int  , Stream* s )
{
	MyString hold_reason;
	int hold_code;
	int hold_subcode;

	s->decode();
	if( !s->get(hold_reason) ||
		!s->get(hold_code) ||
		!s->get(hold_subcode) ||
		!s->end_of_message() )
	{
		dprintf(D_ALWAYS,"Failed to read message from %s in CStarter::remoteHoldCommand()\n", s->peer_description());
		return FALSE;
	}

	if( jic ) {
		jic->holdJob(hold_reason.Value(),hold_code,hold_subcode);
	}

	int reply = 1;
	s->encode();
	if( !s->put(reply) || !s->end_of_message()) {
		dprintf(D_ALWAYS,"Failed to send response to startd in CStarter::remoteHoldCommand()\n");
	}

	if ( this->Hold( ) ) {
		dprintf( D_FULLDEBUG, "Got Hold when no jobs running\n" );
		this->allJobsDone();
		return ( true );
	}	
	return ( false );
}
