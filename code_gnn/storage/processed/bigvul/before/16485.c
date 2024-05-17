check_params()
{
#if defined( HPUX )
	char* tmp;
	if( !(tmp = param("ARCH")) ) {
		fprintf( stderr, "ERROR: %s must know if you are running "
				 "on an HPPA1 or an HPPA2 CPU.\n",
				 myDistro->Get() );
		fprintf( stderr, "Normally, we look in %s for your model.\n",
				 "/opt/langtools/lib/sched.models" );
		fprintf( stderr, "This file lists all HP models and the "
				 "corresponding CPU type.  However,\n" );
		fprintf( stderr, "this file does not exist on your machine "
				 "or your model (%s)\n", sysapi_uname_arch() );
		fprintf( stderr, "was not listed.  You should either explicitly "
				 "set the ARCH parameter\n" );
		fprintf( stderr, "in your config source, or install the "
				 "sched.models file.\n" );
		exit( 1 );
	} else {
		free( tmp );
	}
#endif
}
