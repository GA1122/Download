Init() {
  char * tmp = param( "CRED_SUPER_USERS" );
  if( tmp ) {
    super_users.initializeFromString( tmp );
    free( tmp );
  } else {
#if defined(WIN32)
    super_users.initializeFromString("Administrator");
#else
    super_users.initializeFromString("root");
#endif
  }

  char * spool = param ("SPOOL");

  tmp = param ( "CRED_STORE_DIR" );
  if ( tmp ) {
    cred_store_dir = tmp;
  } else {
    cred_store_dir = dircat (spool, "cred");
  } 
  if ( spool != NULL ) {
	  free (spool);
  }

  tmp = param ( "CRED_INDEX_FILE" );
  if (tmp ) {
    cred_index_file = tmp;
  } else {
    cred_index_file = dircat (cred_store_dir, "cred-index");
  }

  default_cred_expire_threshold = param_integer ("DEFAULT_CRED_EXPIRE_THRESHOLD", 3600);

	CheckCredentials_interval =
		param_integer (
			"CRED_CHECK_INTERVAL",		 
			DEF_CRED_CHECK_INTERVAL		 
	  );

  struct stat stat_buff;
  if (stat (cred_store_dir, &stat_buff)) {
    dprintf (D_ALWAYS, "ERROR: Cred store directory %s does not exist\n", cred_store_dir);
    DC_Exit (1 );
  }

  if (stat (cred_index_file, &stat_buff)) {
    dprintf (D_ALWAYS, "Creating credential index file %s\n", cred_index_file);
    priv_state priv = set_root_priv();
    int fd = safe_open_wrapper_follow(cred_index_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd != -1) {
      close (fd);
      set_priv (priv);
    } else {
      dprintf (D_ALWAYS, "ERROR: Unable to create credential index file %s\n", cred_index_file);
      set_priv (priv);
      DC_Exit (1 );
    }
  } else {
    if ((stat_buff.st_mode & (S_IRWXG | S_IRWXO)) ||
	(stat_buff.st_uid != getuid())) {
      dprintf (D_ALWAYS, "ERROR: Invalid ownership / permissions on credential index file %s\n", 
	       cred_index_file);
      DC_Exit (1 );
    }
  }

}
