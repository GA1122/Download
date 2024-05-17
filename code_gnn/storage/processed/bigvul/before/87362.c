vips_foreign_load_gif_load( VipsForeignLoad *load )
{
	VipsForeignLoadGifClass *class = 
		(VipsForeignLoadGifClass *) VIPS_OBJECT_GET_CLASS( load );
	VipsForeignLoadGif *gif = (VipsForeignLoadGif *) load;
	VipsImage **t = (VipsImage **) 
		vips_object_local_array( VIPS_OBJECT( load ), 4 );

	 
	if( class->open( gif ) )
		return( -1 );

	VIPS_DEBUG_MSG( "vips_foreign_load_gif_load:\n" ); 

	 
	gif->frame = vips_image_new_memory();
	vips_image_init_fields( gif->frame, 
		gif->file->SWidth, gif->file->SHeight, 4, VIPS_FORMAT_UCHAR,
		VIPS_CODING_NONE, VIPS_INTERPRETATION_sRGB, 1.0, 1.0 );
	if( vips_image_write_prepare( gif->frame ) ) 
		return( -1 );

	 
	gif->previous = vips_image_new_memory();
	vips_image_init_fields( gif->previous, 
		gif->file->SWidth, gif->file->SHeight, 4, VIPS_FORMAT_UCHAR,
		VIPS_CODING_NONE, VIPS_INTERPRETATION_sRGB, 1.0, 1.0 );
	if( vips_image_write_prepare( gif->previous ) ) 
		return( -1 );

	 
	t[0] = vips_image_new();
	if( vips_foreign_load_gif_set_header( gif, t[0] ) )
		return( -1 );

	 
	if( vips_image_generate( t[0], 
		NULL, vips_foreign_load_gif_generate, NULL, gif, NULL ) ||
		vips_sequential( t[0], &t[1], 
			"tile_height", VIPS__FATSTRIP_HEIGHT, 
			NULL ) ||
		vips_image_write( t[1], load->real ) )
		return( -1 );

	return( 0 );
}