  main( int    argc,
        char*  argv[] )
  {
    grEvent      event;


    parse_cmdline( &argc, &argv );

#if FREETYPE_MAJOR == 2 && FREETYPE_MINOR == 0 && FREETYPE_PATCH <= 8
    if ( status.debug )
    {
#ifdef FT_DEBUG_LEVEL_TRACE
      FT_SetTraceLevel( trace_any, (FT_Byte)status.trace_level );
#else
      status.trace_level = 0;
#endif
    }
#elif 0
        
        
    if ( status.debug )
    {
      char  temp[32];

      sprintf( temp, "any=%d", status.trace_level );
      setenv( "FT2_DEBUG", temp );
    }
#endif

     
    handle = FTDemo_New( status.encoding );

    FT_Library_SetLcdFilter( handle->library, FT_LCD_FILTER_DEFAULT );

    if ( status.preload )
      FTDemo_Set_Preload( handle, 1 );

    for ( ; argc > 0; argc--, argv++ )
      FTDemo_Install_Font( handle, argv[0] );

    if ( handle->num_fonts == 0 )
      Fatal( "could not find/open any font file" );

    display = FTDemo_Display_New( gr_pixel_mode_rgb24 );
    if ( !display )
      Fatal( "could not allocate display surface" );

    memset( display->fore_color.chroma, 0, 4 );
    memset( display->back_color.chroma, 0xff, 4 );
    grSetTitle( display->surface,
                "FreeType Glyph Viewer - press F1 for help" );

    status.Fail = 0;

    event_font_change( 0 );

    if ( status.lcd_mode >= 0 )
      handle->lcd_mode = status.lcd_mode;

    FTDemo_Update_Current_Flags( handle );

    for ( ;; )
    {
      FTDemo_Display_Clear( display );

      switch ( status.render_mode )
      {
      case RENDER_MODE_ALL:
        error = Render_All( handle->current_font->num_indices,
                            status.Num );
        break;

      case RENDER_MODE_EMBOLDEN:
        error = Render_Embolden( handle->current_font->num_indices,
                                 status.Num );
        break;

      case RENDER_MODE_SLANTED:
        error = Render_Slanted( handle->current_font->num_indices,
                                status.Num );
        break;

      case RENDER_MODE_STROKE:
        error = Render_Stroke( handle->current_font->num_indices,
                               status.Num );
        break;

      case RENDER_MODE_TEXT:
        error = Render_Text( -1, status.Num );
        break;

      case RENDER_MODE_WATERFALL:
        error = Render_Waterfall( status.ptsize );
        break;
      }

      write_header( error );

#if FREETYPE_MAJOR == 2 && FREETYPE_MINOR < 2
      if ( status.dump_cache_stats )
      {
         
        fprintf( stderr, "cache manager [ nodes, bytes, average ] = "
                         " [ %d, %ld, %f ]\n",
                         handle->cache_manager->num_nodes,
                         handle->cache_manager->cur_weight,
                         handle->cache_manager->num_nodes > 0
                           ? handle->cache_manager->cur_weight * 1.0 /
                               handle->cache_manager->num_nodes
                           : 0.0 );
      }
#endif

      status.header = 0;
      grListenSurface( display->surface, 0, &event );
      if ( Process_Event( &event ) )
        break;
    }

    printf( "Execution completed successfully.\n" );
    printf( "Fails = %d\n", status.Fail );

    FTDemo_Display_Done( display );
    FTDemo_Done( handle );
    exit( 0 );       

    return 0;        
  }
