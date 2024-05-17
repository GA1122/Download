CStarter::closeSavedStderr( void )
{
	if( starter_stderr_fd > -1 ) {
		close( starter_stderr_fd );
		starter_stderr_fd = -1;
	}
}
