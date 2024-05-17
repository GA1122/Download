check_job_spool_hierarchy( char const *parent, char const *child, StringList &bad_spool_files )
{
	ASSERT( parent );
	ASSERT( child );


	char *end=NULL;
	strtol(child,&end,10);
	if( !end || *end != '\0' ) {
		return false;  
	}

	std::string topdir;
	sprintf(topdir,"%s%c%s",parent,DIR_DELIM_CHAR,child);
	Directory dir(topdir.c_str(),PRIV_ROOT);
	char const *f;
	while( (f=dir.Next()) ) {

		if( is_ckpt_file(f) ) {
			good_file( topdir.c_str(), f );
			continue;
		}

		if( IsDirectory(dir.GetFullPath()) && !IsSymlink(dir.GetFullPath()) ) {
			if( check_job_spool_hierarchy( topdir.c_str(), f, bad_spool_files ) ) {
				good_file( topdir.c_str(), f );
				continue;
			}
		}

		bad_spool_files.append( dir.GetFullPath() );
	}


	return true;
}
