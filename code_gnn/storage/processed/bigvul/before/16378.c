static int use_append( const char *  , const char *path )
{
	return attr_list_has_file( ATTR_APPEND_FILES, path );
}
