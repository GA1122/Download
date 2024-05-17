process_config_source( const char* file, const char* name,
					   const char* host, int required )
{
	int rval;
	if( access( file, R_OK ) != 0 && !is_piped_command(file)) {
		if( !required) { return; }

		if( !host ) {
			fprintf( stderr, "ERROR: Can't read %s %s\n",
					 name, file );
			exit( 1 );
		}
	} else {
		rval = Read_config( file, ConfigTab, TABLESIZE, EXPAND_LAZY,
							false, extra_info );
		if( rval < 0 ) {
			fprintf( stderr,
					 "Configuration Error Line %d while reading %s %s\n",
					 ConfigLineNo, name, file );
			exit( 1 );
		}
	}
}
