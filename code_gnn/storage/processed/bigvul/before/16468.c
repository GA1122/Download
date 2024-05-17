good_file( const char *dir, const char *name )
{
	if( VerboseFlag ) {
		printf( "%s%c%s - OK\n", dir, DIR_DELIM_CHAR, name );
		dprintf( D_ALWAYS, "%s%c%s - OK\n", dir, DIR_DELIM_CHAR, name );
	}
}
