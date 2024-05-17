static __attribute__ ((noreturn)) void Sys_Exit( int exitCode )
{
	CON_Shutdown( );

#ifndef DEDICATED
	SDL_Quit( );
#endif

	if( exitCode < 2 && com_fullyInitialized )
	{
		Sys_RemovePIDFile( FS_GetCurrentGameDir() );
	}

	NET_Shutdown( );

	Sys_PlatformExit( );

	exit( exitCode );
}
