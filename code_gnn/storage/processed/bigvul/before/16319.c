BaseShadow::checkSwap( void )
{
	int	reserved_swap, free_swap;
	reserved_swap = param_integer( "RESERVED_SWAP", 0 );
	reserved_swap *= 1024;

	if( reserved_swap == 0 ) {
		return;
	}

	free_swap = sysapi_swap_space();

	dprintf( D_FULLDEBUG, "*** Reserved Swap = %d\n", reserved_swap );
	dprintf( D_FULLDEBUG, "*** Free Swap = %d\n", free_swap );

	if( free_swap < reserved_swap ) {
		dprintf( D_ALWAYS, "Not enough reserved swap space\n" );
		DC_Exit( JOB_NO_MEM );
	}
}	
