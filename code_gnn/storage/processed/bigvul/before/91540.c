vips_tracked_open( const char *pathname, int flags, ... )
{
	int fd;
	mode_t mode;
	va_list ap;

	 
	va_start( ap, flags );
	mode = va_arg( ap, int );
	va_end( ap );

	if( (fd = vips__open( pathname, flags, mode )) == -1 )
		return( -1 );

	vips_tracked_init(); 

	g_mutex_lock( vips_tracked_mutex );

	vips_tracked_files += 1;
#ifdef DEBUG_VERBOSE
	printf( "vips_tracked_open: %s = %d (%d)\n", 
		pathname, fd, vips_tracked_files );
#endif  

	g_mutex_unlock( vips_tracked_mutex );

	return( fd );
}