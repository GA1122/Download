CStarter::StarterExit( int code )
{
	FinalCleanup();
#if !defined(WIN32)
	if ( GetEnv( "CONDOR_GLEXEC_STARTER_CLEANUP_FLAG" ) ) {
		exitAfterGlexec( code );
	}
#endif
	DC_Exit( code );
}
