static int use_fetch( const char *  , const char *path )
{
	return attr_list_has_file( ATTR_FETCH_FILES, path );
}
