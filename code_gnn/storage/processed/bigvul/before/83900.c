vips_foreign_load_build( VipsObject *object )
{
	VipsObjectClass *class = VIPS_OBJECT_GET_CLASS( object );
	VipsForeignLoad *load = VIPS_FOREIGN_LOAD( object );
	VipsForeignLoadClass *fclass = VIPS_FOREIGN_LOAD_GET_CLASS( object );

	VipsForeignFlags flags;

#ifdef DEBUG
	printf( "vips_foreign_load_build:\n" );
#endif  

	flags = 0;
	if( fclass->get_flags )
		flags |= fclass->get_flags( load );

	if( (flags & VIPS_FOREIGN_PARTIAL) &&
		(flags & VIPS_FOREIGN_SEQUENTIAL) ) {
		g_warning( "%s", 
			_( "VIPS_FOREIGN_PARTIAL and VIPS_FOREIGN_SEQUENTIAL "
				"both set -- using SEQUENTIAL" ) );
		flags ^= VIPS_FOREIGN_PARTIAL;
	}

	g_object_set( load, "flags", flags, NULL );

	 
	if( (load->flags & VIPS_FOREIGN_SEQUENTIAL) && 
		load->access != VIPS_ACCESS_RANDOM ) 
		load->nocache = TRUE;

	if( VIPS_OBJECT_CLASS( vips_foreign_load_parent_class )->
		build( object ) )
		return( -1 );

	if( load->sequential ) 
		g_warning( "%s", 
			_( "ignoring deprecated \"sequential\" mode -- "
				"please use \"access\" instead" ) ); 

	g_object_set( object, "out", vips_image_new(), NULL ); 

	vips_image_set_string( load->out, 
		VIPS_META_LOADER, class->nickname );

#ifdef DEBUG
	printf( "vips_foreign_load_build: triggering ->header()\n" );
#endif  

	 
	if( fclass->header &&
		fclass->header( load ) ) 
		return( -1 );

	 
	if( fclass->load ) {
#ifdef DEBUG
		printf( "vips_foreign_load_build: delaying read ...\n" );
#endif  

		 
		vips_image_pipelinev( load->out, load->out->dhint, NULL );

		 
		if( vips_image_generate( load->out, 
			vips_foreign_load_start, 
			vips_foreign_load_generate, 
			vips_stop_one, 
			NULL, load ) ) 
			return( -1 );
	}

	 
	if( load->access == VIPS_ACCESS_RANDOM ) 
		(void) vips_image_remove( load->out, VIPS_META_SEQUENTIAL );

	return( 0 );
}
