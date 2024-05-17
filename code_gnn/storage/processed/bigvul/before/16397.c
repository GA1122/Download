CStarter::RemoteShutdownFast(int)
{
	bool fast_in_progress = false;

	if( jic ) {
		fast_in_progress = jic->isFastShutdown();
		jic->gotShutdownFast();
	}
	if( fast_in_progress == false ) {
		return ( this->ShutdownFast( ) );
	}
	else {
		return ( false );
	}
}
