vips_tracked_free( void *s )
{
	 
	void *start = (void *) ((char *) s - 16);
	size_t size = *((size_t *) start);

	g_mutex_lock( vips_tracked_mutex );

#ifdef DEBUG_VERBOSE
	printf( "vips_tracked_free: %p, %zd bytes\n", s, size ); 
#endif  

	if( vips_tracked_allocs <= 0 ) 
		g_warning( "%s", _( "vips_free: too many frees" ) );
	if( vips_tracked_mem < size )
		g_warning( "%s", _( "vips_free: too much free" ) );

	vips_tracked_mem -= size;
	vips_tracked_allocs -= 1;

	g_mutex_unlock( vips_tracked_mutex );

	g_free( start );

	VIPS_GATE_FREE( size ); 
}
