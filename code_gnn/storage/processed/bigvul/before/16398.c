CStarter::RemoteShutdownGraceful( int )
{
	bool graceful_in_progress = false;

	if ( jic ) {
		graceful_in_progress = jic->isGracefulShutdown();
		jic->gotShutdownGraceful();
	}
	if ( graceful_in_progress == false ) {
		return ( this->ShutdownGraceful( ) );
	}
	else {
		return ( false );
	}
}
