check_execute_dir()
{
	time_t now = time(NULL);
	const char	*f;
	bool	busy;
	State	s = get_machine_state();

	switch( s ) {
	case owner_state:	
	case unclaimed_state:
	case matched_state:	
		busy = false;
		break;
	case claimed_state:
	case preempting_state:
		busy = true;
		break;
	default:
		dprintf( D_ALWAYS, 
				 "Error getting startd state, not cleaning execute directory.\n" );
		return;
	}

	ExecuteDirs.rewind();
	char const *Execute;
	while( (Execute=ExecuteDirs.next()) ) {
		Directory dir( Execute, PRIV_ROOT );
		while( (f = dir.Next()) ) {
			if( busy ) {
				good_file( Execute, f );	 
			} else {
				if( dir.GetCreateTime() < now ) {
					bad_file( Execute, f, dir );  
				}
				else {
					dprintf(D_FULLDEBUG, "In %s, found %s with recent "
					        "creation time.  Not removing.\n", Execute, f );
					good_file( Execute, f );  
				}
			}
		}
	}
}
