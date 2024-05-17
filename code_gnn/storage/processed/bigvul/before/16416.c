CStarter::closeSavedStdin( void )
{
	if( starter_stdin_fd > -1 ) {
		close( starter_stdin_fd );
		starter_stdin_fd = -1;
	}
}
