CStarter::removeTempExecuteDir( void )
{
	if( is_gridshell ) {
		return true;
	}

	MyString dir_name = "dir_";
	dir_name += (int)daemonCore->getpid();

#if !defined(WIN32)
	if (condorPrivSepHelper() != NULL) {
		MyString path_name;
		path_name.sprintf("%s/%s", Execute, dir_name.Value());
		if (!privsep_remove_dir(path_name.Value())) {
			dprintf(D_ALWAYS,
			        "privsep_remove_dir failed for %s\n",
			        path_name.Value());
			return false;
		}
		return true;
	}
#endif

	Directory execute_dir( Execute, PRIV_ROOT );
	if ( execute_dir.Find_Named_Entry( dir_name.Value() ) ) {

		chdir(Execute);

		dprintf( D_FULLDEBUG, "Removing %s%c%s\n", Execute,
				 DIR_DELIM_CHAR, dir_name.Value() );
		return execute_dir.Remove_Current_File();
	}
	return true;
}
