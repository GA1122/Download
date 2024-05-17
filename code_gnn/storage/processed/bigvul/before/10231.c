  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    if ( !library )
      return FT_Err_Invalid_Library_Handle;

    memory = library->memory;

     
    if ( library->generic.finalizer )
      library->generic.finalizer( library );

     
    {
      FT_UInt  n;


      for ( n = 0; n < library->num_modules; n++ )
      {
        FT_Module  module = library->modules[n];
        FT_List    faces;


        if ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
          continue;

        faces = &FT_DRIVER(module)->faces_list;
        while ( faces->head )
        {
          FT_Done_Face( FT_FACE( faces->head->data ) );
          if ( faces->head )
            FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
        }
      }
    }

     
#if 1
     
     
     
    while ( library->num_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->num_modules - 1] );
#else
    {
      FT_UInt  n;


      for ( n = 0; n < library->num_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        if ( module )
        {
          Destroy_Module( module );
          library->modules[n] = 0;
        }
      }
    }
#endif

     
    FT_FREE( library->raster_pool );
    library->raster_pool_size = 0;

#ifdef FT_CONFIG_OPTION_PIC
     
    ft_pic_container_destroy( library );
#endif

    FT_FREE( library );
    return FT_Err_Ok;
  }
