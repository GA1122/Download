real_config(char* host, int wantsQuiet, bool wantExtraInfo)
{
	char* config_source = NULL;
	char* tmp = NULL;
	int scm;

	static bool first_time = true;
	if( first_time ) {
		first_time = false;
		init_config(wantExtraInfo);
	} else {
		clear_config();
		if (wantExtraInfo) {
			extra_info = new ExtraParamTable();
		} else {
			extra_info = new DummyExtraParamTable();
		}
	}

	dprintf( D_CONFIG, "config: using subsystem '%s', local '%s'\n",
			 get_mySubSystem()->getName(), get_mySubSystem()->getLocalName("") );

		 
	scm = SetSyscalls( SYS_LOCAL | SYS_UNRECORDED );

	init_tilde();

	if( tilde ) {
		insert( "TILDE", tilde, ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("TILDE");

	} else {
	}

	sysapi_clear_network_device_info_cache();

	fill_attributes();


	char* env = getenv( EnvGetName(ENV_CONFIG) );
	if( env && strcasecmp(env, "ONLY_ENV") == MATCH ) {
		have_config_source = false;
	}

	if( have_config_source && ! (config_source = find_global()) ) {
		if( wantsQuiet ) {
			fprintf( stderr, "%s error: can't find config source.\n",
					 myDistro->GetCap() );
			exit( 1 );
		}
		fprintf(stderr,"\nNeither the environment variable %s_CONFIG,\n",
				myDistro->GetUc() );
#	  if defined UNIX
		fprintf(stderr,"/etc/%s/, nor ~%s/ contain a %s_config source.\n",
				myDistro->Get(), myDistro->Get(), myDistro->Get() );
#	  elif defined WIN32
		fprintf(stderr,"nor the registry contains a %s_config source.\n", myDistro->Get() );
#	  else
#		error "Unknown O/S"
#	  endif
		fprintf( stderr,"Either set %s_CONFIG to point to a valid config "
				"source,\n", myDistro->GetUc() );
#	  if defined UNIX
		fprintf( stderr,"or put a \"%s_config\" file in /etc/%s or ~%s/\n",
				 myDistro->Get(), myDistro->Get(), myDistro->Get() );
#	  elif defined WIN32
		fprintf( stderr,"or put a \"%s_config\" source in the registry at:\n"
				 " HKEY_LOCAL_MACHINE\\Software\\%s\\%s_CONFIG",
				 myDistro->Get(), myDistro->Get(), myDistro->GetUc() );
#	  else
#		error "Unknown O/S"
#	  endif
		fprintf( stderr, "Exiting.\n\n" );
		exit( 1 );
	}

	if( have_config_source ) {
		process_config_source( config_source, "global config source", NULL, true );
		global_config_source = config_source;
		free( config_source );
		config_source = NULL;
	}

	if( host ) {
		insert( "HOSTNAME", host, ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("HOSTNAME");
	} else {
		insert( "HOSTNAME", get_local_hostname().Value(), ConfigTab,
				TABLESIZE );
		extra_info->AddInternalParam("HOSTNAME");
	}
	insert( "FULL_HOSTNAME", get_local_fqdn().Value(), ConfigTab, TABLESIZE );
	extra_info->AddInternalParam("FULL_HOSTNAME");

	if( tilde ) {
		insert( "TILDE", tilde, ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("TILDE");
	}

	char *dirlist = param("LOCAL_CONFIG_DIR");
	if(dirlist) {
		process_directory(dirlist, host);
	}
	process_locals( "LOCAL_CONFIG_FILE", host );

	char* newdirlist = param("LOCAL_CONFIG_DIR");
	if(newdirlist) {
		if (dirlist) {
			if(strcmp(dirlist, newdirlist) ) {
				process_directory(newdirlist, host);
			}
		}
		else {
			process_directory(newdirlist, host);
		}
	}

	if(dirlist) { free(dirlist); dirlist = NULL; }
	if(newdirlist) { free(newdirlist); newdirlist = NULL; }

    if ( param_boolean("NET_REMAP_ENABLE", false) ) {
        condor_net_remap_config();
    }
			
	char **my_environ = GetEnviron();
	for( int i = 0; my_environ[i]; i++ ) {
		char magic_prefix[MAX_DISTRIBUTION_NAME + 3];	 
		strcpy( magic_prefix, "_" );
		strcat( magic_prefix, myDistro->Get() );
		strcat( magic_prefix, "_" );
		int prefix_len = strlen( magic_prefix );

		if( strncasecmp( my_environ[i], magic_prefix, prefix_len ) != 0 ) {
			continue;
		}

		char *varname = strdup( my_environ[i] );
		if( !varname ) {
			EXCEPT( "Out of memory in %s:%d\n", __FILE__, __LINE__ );
		}

		int equals_offset = strchr( varname, '=' ) - varname;
		varname[equals_offset] = '\0';
		char *varvalue = varname + equals_offset + 1;
		char *macro_name = varname + prefix_len;

		if( !strcmp( macro_name, "START_owner" ) ) {
			MyString ownerstr;
			ownerstr.sprintf( "Owner == \"%s\"", varvalue );
			insert( "START", ownerstr.Value(), ConfigTab, TABLESIZE );
			extra_info->AddEnvironmentParam("START");
		}
		else if( macro_name[0] != '\0' ) {
			insert( macro_name, varvalue, ConfigTab, TABLESIZE );
			extra_info->AddEnvironmentParam(macro_name);
		}

		free( varname );
	}

	reinsert_specials( host );

	process_dynamic_configs();

	if (config_source) {
		free( config_source );
	}

	init_network_interfaces(TRUE);

	if( (tmp = param("DEFAULT_DOMAIN_NAME")) ) {
		free( tmp );
		init_local_hostname();
	}



	init_local_hostname();

	reinsert_specials( host );

	check_domain_attributes();

	check_params();

	condor_except_should_dump_core( param_boolean("ABORT_ON_EXCEPTION", false) );

	condor_auth_config( false );

	ConfigConvertDefaultIPToSocketIP();

	condor_fsync_on = param_boolean("CONDOR_FSYNC", true);
	if(!condor_fsync_on)
		dprintf(D_FULLDEBUG, "FSYNC while writing user logs turned off.\n");

	(void)SetSyscalls( scm );
}
