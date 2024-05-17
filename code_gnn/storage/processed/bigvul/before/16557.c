EventHandler::block_events( sigset_t &sigset )
{
	if( !is_installed ) {
		EXCEPT( "ERROR EventHandler::block_events(), not installed");
	}
	sigprocmask( SIG_BLOCK, &sigset, 0 );
}
