CStarter::closeSavedStdout( void )
{
	if( starter_stdout_fd > -1 ) {
		close( starter_stdout_fd );
		starter_stdout_fd = -1;
	}
}
