vips_tracked_close( int fd )
{
	int result;

	g_mutex_lock( vips_tracked_mutex );

	g_assert( vips_tracked_files > 0 );

	vips_tracked_files -= 1;
#ifdef DEBUG_VERBOSE
	printf( "vips_tracked_close: %d (%d)\n", fd, vips_tracked_files );
#endif  

	g_mutex_unlock( vips_tracked_mutex );

	result = close( fd );

	return( result );
}