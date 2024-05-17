is_ccb_file( const char *name )
{
	if( strstr(name,".ccb_reconnect") ) {
		return TRUE;
	}
	return FALSE;
}
