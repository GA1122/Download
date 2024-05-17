vips_foreign_load_gif_extension( VipsForeignLoadGif *gif )
{
	GifByteType *extension;
	int ext_code;

	VIPS_DEBUG_MSG( "vips_foreign_load_gif_extension:\n" ); 

	if( DGifGetExtension( gif->file, &ext_code, &extension ) == 
		GIF_ERROR ) {
		vips_foreign_load_gif_error( gif ); 
		return( -1 ); 
	}

	if( extension &&
		ext_code == GRAPHICS_EXT_FUNC_CODE &&
		extension[0] == 4 ) { 
		 
		gif->transparency = -1;
		if( extension[1] & 0x1 ) 
			gif->transparency = extension[4];

		 
		gif->dispose = (extension[1] >> 2) & 0x7;
		VIPS_DEBUG_MSG( "vips_foreign_load_gif_extension: "
			"dispose = %d\n", gif->dispose ); 
	}

	while( extension != NULL ) 
		if( vips_foreign_load_gif_ext_next( gif, &extension ) )
			return( -1 ); 

	return( 0 );
}
