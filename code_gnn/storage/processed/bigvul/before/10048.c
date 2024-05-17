  main( int     argc,
        char**  argv )
  {
    grEvent  event;


    parse_cmdline( &argc, &argv );

     
    handle = FTDemo_New( status.encoding );

    handle->use_sbits = 0;
    FTDemo_Update_Current_Flags( handle );

    for ( ; argc > 0; argc--, argv++ )
    {
      error = FTDemo_Install_Font( handle, argv[0] );

      if ( error )
      {
        fprintf( stderr, "failed to install %s", argv[0] );
        if ( error == FT_Err_Invalid_CharMap_Handle )
          fprintf( stderr, ": missing valid charmap\n" );
        else
          fprintf( stderr, "\n" );
      }
    }

    if ( handle->num_fonts == 0 )
      PanicZ( "could not open any font file" );

    display = FTDemo_Display_New( gr_pixel_mode_gray );
    display->back_color.value = 0;
    display->fore_color.value = 0xff;

    if ( !display )
      PanicZ( "could not allocate display surface" );

    grSetTitle( display->surface,
                "FreeType String Viewer - press F1 for help" );

    event_gamma_change( 0 );
    event_font_change( 0 );
    status.header = 0;

    for ( ;; )
    {
      FTDemo_Display_Clear( display );

      switch ( status.render_mode )
      {
      case RENDER_MODE_STRING:
        status.sc.center = 1L << 15;
        error = FTDemo_String_Draw( handle, display,
                                    &status.sc,
                                    display->bitmap->width / 2,
                                    display->bitmap->rows / 2 );
        break;

      case RENDER_MODE_KERNCMP:
        {
          FTDemo_String_Context  sc = status.sc;
          FT_Int                 x, y;
          FT_UInt                height;


          x = 55;

           
          height = status.ptsize * status.res / 72;
          if ( height < CELLSTRING_HEIGHT )
            height = CELLSTRING_HEIGHT;

           
          sc.center         = 0;
          sc.kerning_mode   = 0;
          sc.kerning_degree = 0;
          sc.vertical       = 0;
          sc.matrix         = NULL;

          y = CELLSTRING_HEIGHT * 2 + display->bitmap->rows / 4 + height;
          grWriteCellString( display->bitmap, 5,
                             y - ( height + CELLSTRING_HEIGHT ) / 2,
                             "none", display->fore_color );
          error = FTDemo_String_Draw( handle, display, &sc, x, y );


           
          sc.kerning_degree = status.sc.kerning_degree;

          y += height;
          grWriteCellString( display->bitmap, 5,
                             y - ( height + CELLSTRING_HEIGHT ) / 2,
                             "track", display->fore_color );
          error = FTDemo_String_Draw( handle, display, &sc, x, y );


           
          sc.kerning_mode      = status.sc.kerning_mode;

          y += height;
          grWriteCellString( display->bitmap, 5,
                             y - ( height + CELLSTRING_HEIGHT ) / 2,
                             "both", display->fore_color );
          error = FTDemo_String_Draw( handle, display, &sc, x, y );
        }
        break;
      }

      if ( !error && status.sc.gamma_ramp )
        gamma_ramp_draw( status.gamma_ramp, display->bitmap );

      write_header( error );

      status.header = 0;
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
