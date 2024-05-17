find_file(const char *env_name, const char *file_name)
{
	char* config_source = NULL;
	char* env = NULL;
	int fd = 0;

	if( env_name && (env = getenv( env_name )) ) {
		config_source = strdup( env );
		StatInfo si( config_source );
		switch( si.Error() ) {
		case SIGood:
			if( si.IsDirectory() ) {
				fprintf( stderr, "File specified in %s environment "
						 "variable:\n\"%s\" is a directory.  "
						 "Please specify a file.\n", env_name,
						 config_source );
				free( config_source );
				config_source = NULL;
				exit( 1 );
			}
			return config_source;
			break;
		case SINoFile:
			if (!is_piped_command(config_source) ||
				!is_valid_command(config_source)) {

				fprintf( stderr, "File specified in %s environment "
						 "variable:\n\"%s\" does not exist.\n",
						 env_name, config_source );
				free( config_source );
				exit( 1 );
				break;
			}
			return config_source;

		case SIFailure:
			fprintf( stderr, "Cannot stat file specified in %s "
					 "environment variable:\n\"%s\", errno: %d\n",
					 env_name, config_source, si.Errno() );
			free( config_source );
			exit( 1 );
			break;
		}
	}

# ifdef UNIX

	if (!config_source) {
		int locations_length = 4;
		MyString locations[locations_length];
		locations[0].sprintf( "/etc/%s/%s", myDistro->Get(), file_name );
		locations[1].sprintf( "/usr/local/etc/%s", file_name );
		if (tilde) {
			locations[2].sprintf( "%s/%s", tilde, file_name );
		}
		char *globus_location;
		if ((globus_location = getenv("GLOBUS_LOCATION"))) {
			locations[3].sprintf( "%s/etc/%s", globus_location, file_name );
		}

		int ctr;	
		for (ctr = 0 ; ctr < locations_length; ctr++) {
			if (!locations[ctr].IsEmpty()) {
				config_source = strdup(locations[ctr].Value());
				if ((fd = safe_open_wrapper_follow(config_source, O_RDONLY)) < 0) {
					free(config_source);
					config_source = NULL;
				} else {
					close(fd);
					dprintf(D_FULLDEBUG, "Reading condor configuration "
							"from '%s'\n", config_source);
					break;
				}
			}
		}  
	}  

# elif defined WIN32	 
	HKEY	handle;
	char	regKey[256];

	snprintf( regKey, 256, "Software\\%s", myDistro->GetCap() );
	if ( !config_source && RegOpenKeyEx(HKEY_LOCAL_MACHINE, regKey,
		0, KEY_READ, &handle) == ERROR_SUCCESS ) {

		char the_path[MAX_PATH];
		DWORD valType;
		DWORD valSize = MAX_PATH - 2;

		the_path[0] = '\0';
		if ( RegQueryValueEx(handle, env_name, 0,
			&valType, (unsigned char *)the_path, &valSize) == ERROR_SUCCESS ) {

			if ( valType == REG_SZ && the_path[0] ) {
				config_source = strdup(the_path);

				if ( strncmp(config_source, "\\\\", 2 ) == 0 ) {
					NETRESOURCE nr;
					nr.dwType = RESOURCETYPE_DISK;
					nr.lpLocalName = NULL;
					nr.lpRemoteName = condor_dirname(config_source);
					nr.lpProvider = NULL;
					
					if ( NO_ERROR != WNetAddConnection2(
										&nr,    
										NULL,   
										NULL,   
										0       
						) ) {

						if ( GetLastError() == ERROR_INVALID_PASSWORD ) {
							WNetAddConnection2(
										&nr,    
										"",     
										NULL,   
										0       
							);
						}

					}

					if (nr.lpRemoteName) {
						free(nr.lpRemoteName);
					}
				}

				if( !(is_piped_command(config_source) &&
					  is_valid_command(config_source)) &&
					(fd = safe_open_wrapper_follow( config_source, O_RDONLY)) < 0 ) {

					free( config_source );
					config_source = NULL;
				} else {
					if (fd != 0) {
						close( fd );
					}
				}
			}
		}

		RegCloseKey(handle);
	}
# else
#	error "Unknown O/S"
# endif		 

	return config_source;
}
