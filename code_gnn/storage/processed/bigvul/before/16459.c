bad_file( const char *dirpath, const char *name, Directory & dir )
{
	MyString	pathname;
	MyString	buf;

	if( is_relative_to_cwd( name ) ) {
	pathname.sprintf( "%s%c%s", dirpath, DIR_DELIM_CHAR, name );
	}
	else {
		pathname = name;
	}

	if( VerboseFlag ) {
		printf( "%s - BAD\n", pathname.Value() );
		dprintf( D_ALWAYS, "%s - BAD\n", pathname.Value() );
	}

	if( RmFlag ) {
		bool removed = dir.Remove_Full_Path( pathname.Value() );
		if( !removed && privsep_enabled() ) {
			removed = privsep_remove_dir( pathname.Value() );
			if( VerboseFlag ) {
				if( removed ) {
					dprintf( D_ALWAYS, "%s - failed to remove directly, but succeeded via privsep switchboard\n", pathname.Value() );
					printf( "%s - failed to remove directly, but succeeded via privsep switchboard\n", pathname.Value() );
				}
			}
		}
		if( removed ) {
			buf.sprintf( "%s - Removed", pathname.Value() );
		} else {
			buf.sprintf( "%s - Can't Remove", pathname.Value() );
		}
	} else {
		buf.sprintf( "%s - Not Removed", pathname.Value() );
	}
	BadFiles->append( buf.Value() );
}
