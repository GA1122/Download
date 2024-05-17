CStarter::RemoteRemove( int )
{
	if( jic ) {
		jic->gotRemove();
	}
	if ( this->Remove( ) ) {
		dprintf( D_FULLDEBUG, "Got Remove when no jobs running\n" );
		this->allJobsDone();
		return ( true );
	}	
	return ( false );
}
