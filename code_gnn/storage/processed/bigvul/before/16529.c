set_persistent_config(char *admin, char *config)
{
	int fd, rval;
	char *tmp;
	MyString filename;
	MyString tmp_filename;
	priv_state priv;

	if (!admin || !admin[0] || !enable_persistent) {
		if (!enable_persistent) {
			dprintf( D_ALWAYS, "set_persistent_config(): "
				"ENABLE_PERSISTENT_CONFIG is false. "
				"Not setting persistent config file param: "
				"Name = %s, Value = %s\n",
				admin?admin:"(null pointer)",
				config?config:"(null pointer)");
		}
		if (admin)  { free(admin);  }
		if (config) { free(config); }
		return -1;
	}

	init_dynamic_config();
	if( ! toplevel_persistent_config.Length() ) {
		EXCEPT( "Impossible: programmer error: toplevel_persistent_config "
				"is 0-length, but we already initialized, enable_persistent "
				"is TRUE, and set_persistent_config() has been called" );
	}

	priv = set_root_priv();
	if (config && config[0]) {	 
		filename.sprintf( "%s.%s", toplevel_persistent_config.Value(), admin );
		tmp_filename.sprintf( "%s.tmp", filename.Value() );
		do {
			unlink( tmp_filename.Value() );
			fd = safe_open_wrapper_follow( tmp_filename.Value(), O_WRONLY|O_CREAT|O_EXCL, 0644 );
		} while (fd == -1 && errno == EEXIST);
		if( fd < 0 ) {
			dprintf( D_ALWAYS, "safe_open_wrapper(%s) returned %d '%s' (errno %d) in "
					 "set_persistent_config()\n", tmp_filename.Value(),
					 fd, strerror(errno), errno );
			ABORT;
		}
		if (write(fd, config, strlen(config)) != (ssize_t)strlen(config)) {
			dprintf( D_ALWAYS, "write() failed with '%s' (errno %d) in "
					 "set_persistent_config()\n", strerror(errno), errno );
			ABORT;
		}
		if (close(fd) < 0) {
			dprintf( D_ALWAYS, "close() failed with '%s' (errno %d) in "
					 "set_persistent_config()\n", strerror(errno), errno );
			ABORT;
		}
		
		if (rotate_file(tmp_filename.Value(), filename.Value()) < 0) {
			dprintf( D_ALWAYS, "rotate_file(%s,%s) failed with '%s' "
					 "(errno %d) in set_persistent_config()\n",
					 tmp_filename.Value(), filename.Value(),
					 strerror(errno), errno );
			ABORT;
		}
	
		if (!PersistAdminList.contains(admin)) {
			PersistAdminList.append(admin);
		} else {
			free(admin);
			free(config);
			set_priv(priv);
			return 0;		 
		}

	} else {					 

		PersistAdminList.remove(admin);
		if (config) {
			free(config);
			config = NULL;
		}
	}		

	tmp_filename.sprintf( "%s.tmp", toplevel_persistent_config.Value() );
	do {
		unlink( tmp_filename.Value() );
		fd = safe_open_wrapper_follow( tmp_filename.Value(), O_WRONLY|O_CREAT|O_EXCL, 0644 );
	} while (fd == -1 && errno == EEXIST);
	if( fd < 0 ) {
		dprintf( D_ALWAYS, "safe_open_wrapper(%s) returned %d '%s' (errno %d) in "
				 "set_persistent_config()\n", tmp_filename.Value(),
				 fd, strerror(errno), errno );
		ABORT;
	}
	const char param[] = "RUNTIME_CONFIG_ADMIN = ";
	if (write(fd, param, strlen(param)) != (ssize_t)strlen(param)) {
		dprintf( D_ALWAYS, "write() failed with '%s' (errno %d) in "
				 "set_persistent_config()\n", strerror(errno), errno );
		ABORT;
	}
	PersistAdminList.rewind();
	bool first_time = true;
	while( (tmp = PersistAdminList.next()) ) {
		if (!first_time) {
			if (write(fd, ", ", 2) != 2) {
				dprintf( D_ALWAYS, "write() failed with '%s' (errno %d) in "
						 "set_persistent_config()\n", strerror(errno), errno );
				ABORT;
			}
		} else {
			first_time = false;
		}
		if (write(fd, tmp, strlen(tmp)) != (ssize_t)strlen(tmp)) {
			dprintf( D_ALWAYS, "write() failed with '%s' (errno %d) in "
					 "set_persistent_config()\n", strerror(errno), errno );
			ABORT;
		}
	}
	if (write(fd, "\n", 1) != 1) {
		dprintf( D_ALWAYS, "write() failed with '%s' (errno %d) in "
				 "set_persistent_config()\n", strerror(errno), errno );
		ABORT;
	}
	if (close(fd) < 0) {
		dprintf( D_ALWAYS, "close() failed with '%s' (errno %d) in "
				 "set_persistent_config()\n", strerror(errno), errno );
		ABORT;
	}
	
	rval = rotate_file( tmp_filename.Value(),
						toplevel_persistent_config.Value() );
	if (rval < 0) {
		dprintf( D_ALWAYS, "rotate_file(%s,%s) failed with '%s' (errno %d) "
				 "in set_persistent_config()\n", tmp_filename.Value(),
				 filename.Value(), strerror(errno), errno );
		ABORT;
	}

	if (!config || !config[0]) {
		filename.sprintf( "%s.%s", toplevel_persistent_config.Value(), admin );
		unlink( filename.Value() );
		if (PersistAdminList.number() == 0) {
			unlink( toplevel_persistent_config.Value() );
		}
	}

	set_priv( priv );
	free( admin );
	if (config) { free( config ); }
	return 0;
}
