  main( int    argc,
        char*  argv[] )
  {
    grEvent      event;

    display = FTDemo_Display_New( gr_pixel_mode_rgb24 );
    if ( !display )
      Fatal( "could not allocate display surface" );

    memset( display->fore_color.chroma, 0, 4 );
    memset( display->back_color.chroma, 0xff, 4 );
    grSetTitle( display->surface, "FreeType Glyph Grid Viewer - press F1 for help" );

    grid_status_init( &status, display );

    parse_cmdline( &argc, &argv );

     
    handle = FTDemo_New( FT_ENCODING_NONE );

    for ( ; argc > 0; argc--, argv++ )
      FTDemo_Install_Font( handle, argv[0] );

    if ( handle->num_fonts == 0 )
      Fatal( "could not find/open any font file" );

    printf( "ptsize =%g \n", status.ptsize/64.0 );
    FTDemo_Set_Current_Charsize( handle, status.ptsize, status.res );
    FTDemo_Update_Current_Flags( handle );

    event_font_change( 0 );

    grid_status_rescale_initial( &status, handle );

    _af_debug = 1;

    for ( ;; )
    {
      FTDemo_Display_Clear( display );

      grid_status_draw_grid( &status );

      if ( status.do_outline || status.do_dots )
        grid_status_draw_outline( &status, handle, display );

      write_header( 0 );

      grListenSurface( display->surface, 0, &event );
      if ( Process_Event( &event ) )
        break;
    }

    printf( "Execution completed successfully.\n" );

    FTDemo_Display_Done( display );
    FTDemo_Done( handle );
    exit( 0 );       

    return 0;        
  }
