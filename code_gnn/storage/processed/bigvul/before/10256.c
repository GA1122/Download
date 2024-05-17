  FT_New_Library( FT_Memory    memory,
                  FT_Library  *alibrary )
  {
    FT_Library  library = 0;
    FT_Error    error;


    if ( !memory )
      return FT_Err_Invalid_Argument;

#ifdef FT_DEBUG_LEVEL_ERROR
     
    ft_debug_init();
#endif

     
    if ( FT_NEW( library ) )
      return error;

    library->memory = memory;

#ifdef FT_CONFIG_OPTION_PIC
     
    error = ft_pic_container_init( library );
    if ( error )
      goto Fail;
#endif

     
    library->raster_pool_size = FT_RENDER_POOL_SIZE;
#if FT_RENDER_POOL_SIZE > 0
    if ( FT_ALLOC( library->raster_pool, FT_RENDER_POOL_SIZE ) )
      goto Fail;
#endif

    library->version_major = FREETYPE_MAJOR;
    library->version_minor = FREETYPE_MINOR;
    library->version_patch = FREETYPE_PATCH;

     
    *alibrary = library;

    return FT_Err_Ok;

  Fail:
#ifdef FT_CONFIG_OPTION_PIC
    ft_pic_container_destroy( library );
#endif
    FT_FREE( library );
    return error;
  }