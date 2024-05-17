init_dynamic_config()
{
	static bool initialized = false;

	if( initialized ) {
		return;
	}

	enable_runtime = param_boolean( "ENABLE_RUNTIME_CONFIG", false );
	enable_persistent = param_boolean( "ENABLE_PERSISTENT_CONFIG", false );
	initialized = true;

	if( !enable_persistent ) {
		return;
	}

	char* tmp;

	MyString filename_parameter;
	filename_parameter.sprintf( "%s_CONFIG", get_mySubSystem()->getName() );
	tmp = param( filename_parameter.Value() );
	if( tmp ) {
		toplevel_persistent_config = tmp;
		free( tmp );
		return;
	}

	tmp = param( "PERSISTENT_CONFIG_DIR" );

	if( !tmp ) {
		if ( get_mySubSystem()->isClient( ) || !have_config_source ) {
				 
			return;
		} else {
			fprintf( stderr, "%s error: ENABLE_PERSISTENT_CONFIG is TRUE, "
					 "but neither %s nor PERSISTENT_CONFIG_DIR is "
					 "specified in the configuration file\n",
					 myDistro->GetCap(), filename_parameter.Value() );
			exit( 1 );
		}
	}
	toplevel_persistent_config.sprintf( "%s%c.config.%s", tmp,
										DIR_DELIM_CHAR,
										get_mySubSystem()->getName() );
	free(tmp);
}
