static int use_compress( const char *  , const char *path )
{
	return attr_list_has_file( ATTR_COMPRESS_FILES, path );
}
