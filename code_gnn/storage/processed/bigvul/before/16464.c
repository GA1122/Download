check_spool_dir()
{
    unsigned int	history_length, startd_history_length;
	const char  	*f;
    const char      *history, *startd_history;
	Directory  		dir(Spool, PRIV_ROOT);
	StringList 		well_known_list, bad_spool_files;
	Qmgr_connection *qmgr;

	if ( ValidSpoolFiles == NULL ) {
		dprintf( D_ALWAYS, "Not cleaning spool directory: No VALID_SPOOL_FILES defined\n");
		return;
	}

    history = param("HISTORY");
    history = condor_basename(history);  
    history_length = strlen(history);

    startd_history = param("STARTD_HISTORY");
   	startd_history = condor_basename(startd_history);
   	startd_history_length = strlen(startd_history);

	well_known_list.initializeFromString (ValidSpoolFiles);
	well_known_list.append( "job_queue.log" );
	well_known_list.append( "job_queue.log.tmp" );
	well_known_list.append( "spool_version" );
	well_known_list.append( "Accountant.log" );
	well_known_list.append( "Accountantnew.log" );
	well_known_list.append( "local_univ_execute" );
	well_known_list.append( "EventdShutdownRate.log" );
	well_known_list.append( "OfflineLog" );
	well_known_list.append( "SCHEDD.lock" );
	well_known_list.append( ".quillwritepassword" );
	well_known_list.append( ".pgpass" );
	
	if (!(qmgr = ConnectQ (0))) {
		dprintf( D_ALWAYS, "Not cleaning spool directory: Can't contact schedd\n" );
		return;
	}

	while( (f = dir.Next()) ) {
		if( well_known_list.contains(f) ) {
			good_file( Spool, f );
			continue;
		}
		if( !strncmp(f,"job_queue.log",13) ) {
			good_file( Spool, f );
			continue;
		}
        if (   strlen(f) >= history_length 
            && strncmp(f, history, history_length) == 0) {
            good_file( Spool, f );
            continue;
        }

		if ( startd_history_length > 0 &&
			strlen(f) >= startd_history_length &&
			strncmp(f, startd_history, startd_history_length) == 0) {

            good_file( Spool, f );
            continue;
		}

		if( is_valid_shared_exe(f) ) {
			good_file( Spool, f );
			continue;
		}

		if( is_ckpt_file(f) ) {
			good_file( Spool, f );
			continue;
		}

		if ( is_myproxy_file( f ) ) {
			good_file( Spool, f );
			continue;
		}

		if ( is_ccb_file( f ) ) {
			good_file( Spool, f );
			continue;
		}

		if( IsDirectory( dir.GetFullPath() ) && !IsSymlink( dir.GetFullPath() ) ) {
			if( check_job_spool_hierarchy( Spool, f, bad_spool_files ) ) {
				good_file( Spool, f );
				continue;
			}
		}

		bad_spool_files.append( f );
	}

	if( DisconnectQ(qmgr) ) {
		bad_spool_files.rewind();
		while( (f = bad_spool_files.next()) ) {
			bad_file( Spool, f, dir );
		}
	} else {
		dprintf( D_ALWAYS, 
				 "Error disconnecting from job queue, not deleting spool files.\n" );
	}
}
