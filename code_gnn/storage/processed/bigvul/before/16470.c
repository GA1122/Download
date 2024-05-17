init_params()
{
	Spool = param("SPOOL");
    if( Spool == NULL ) {
        EXCEPT( "SPOOL not specified in config file\n" );
    }

	Log = param("LOG");
    if( Log == NULL ) {
        EXCEPT( "LOG not specified in config file\n" );
    }

	DaemonSockDir = param("DAEMON_SOCKET_DIR");
	if( DaemonSockDir == NULL ) {
		EXCEPT("DAEMON_SOCKET_DIR not defined\n");
	}

	char *Execute = param("EXECUTE");
	if( Execute ) {
		ExecuteDirs.append(Execute);
		free(Execute);
		Execute = NULL;
	}
	ExtArray<ParamValue> *params = param_all();
	for( int p=params->length(); p--; ) {
		char const *name = (*params)[p].name.Value();
		char *tail = NULL;
		if( strncasecmp( name, "SLOT", 4 ) != 0 ) continue;
		strtol( name+4, &tail, 10 );
		if( tail <= name || strcasecmp( tail, "_EXECUTE" ) != 0 ) continue;

		Execute = param(name);
		if( Execute ) {
			if( !ExecuteDirs.contains( Execute ) ) {
				ExecuteDirs.append( Execute );
			}
			free( Execute );
		}
	}
	delete params;

    if( (PreenAdmin = param("PREEN_ADMIN")) == NULL ) {
		if( (PreenAdmin = param("CONDOR_ADMIN")) == NULL ) {
			EXCEPT( "CONDOR_ADMIN not specified in config file" );
		}
    }

	ValidSpoolFiles = param("VALID_SPOOL_FILES");

	InvalidLogFiles = param("INVALID_LOG_FILES");

	if( (MailPrg = param("MAIL")) == NULL ) {
		EXCEPT ( "MAIL not specified in config file" );
	}
}
