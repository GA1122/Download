file_add_class( VipsForeignClass *class, GSList **files )
{
	 
	*files = g_slist_append( *files, class );

	return( NULL );
}
