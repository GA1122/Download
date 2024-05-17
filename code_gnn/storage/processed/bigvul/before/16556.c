EventHandler::allow_events( sigset_t &sigset )
{
	if( !is_installed ) {
		EXCEPT( "ERROR EventHandler::allow_events(), not installed");
	} 
	sigprocmask( SIG_UNBLOCK, &sigset, 0 );
}
