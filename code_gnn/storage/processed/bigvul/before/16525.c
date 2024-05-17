process_persistent_configs()
{
	char *tmp = NULL;
	int rval;
	bool processed = false;

	if( access( toplevel_persistent_config.Value(), R_OK ) == 0 &&
		PersistAdminList.number() == 0 )
	{
		processed = true;

		rval = Read_config( toplevel_persistent_config.Value(), ConfigTab,
							TABLESIZE, EXPAND_LAZY, true, extra_info );
		if (rval < 0) {
			dprintf( D_ALWAYS, "Configuration Error Line %d while reading "
					 "top-level persistent config source: %s\n",
					 ConfigLineNo, toplevel_persistent_config.Value() );
			exit(1);
		}

		tmp = param ("RUNTIME_CONFIG_ADMIN");
		if (tmp) {
			PersistAdminList.initializeFromString(tmp);
			free(tmp);
		}
	}

	PersistAdminList.rewind();
	while ((tmp = PersistAdminList.next())) {
		processed = true;
		MyString config_source;
		config_source.sprintf( "%s.%s", toplevel_persistent_config.Value(),
							   tmp );
		rval = Read_config( config_source.Value(), ConfigTab, TABLESIZE,
							 EXPAND_LAZY, true, extra_info );
		if (rval < 0) {
			dprintf( D_ALWAYS, "Configuration Error Line %d "
					 "while reading persistent config source: %s\n",
					 ConfigLineNo, config_source.Value() );
			exit(1);
		}
	}
	return (int)processed;
}
