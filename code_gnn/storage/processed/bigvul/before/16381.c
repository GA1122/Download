static int use_local_access( const char *file )
{
	return
		!strcmp(file,"/dev/null") ||
		!strcmp(file,"/dev/zero") ||
		attr_list_has_file( ATTR_LOCAL_FILES, file );
}
