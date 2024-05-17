  main( int  argc,
        char**  argv )
  {
    char**          argend = argv + argc;
    ADisplayRec     adisplay[1];
    RenderStateRec  state[1];
    DisplayRec      display[1];
    int             resolution = -1;
    double          size       = -1;
    const char*     textfile   = NULL;
    unsigned char*  text       = (unsigned char*)default_text;


     
    ++argv;
    while ( argv < argend && argv[0][0] == '-' )
    {
      char*  arg = argv[0];


      switch (arg[1])
      {
      case 'r':
        arg = get_option_arg( arg, &argv, argend );
        resolution = atoi( arg );
        break;

      case 's':
        arg = get_option_arg( arg, &argv, argend );
        size = atof( arg );
        break;

      case 'f':
        arg      = get_option_arg( arg, &argv, argend );
        textfile = arg;
        break;

      default:
        usage();
      }
      argv++;
    }

    if ( argv >= argend )
      usage();

     
    if ( textfile != NULL )
    {
      FILE*  tfile = fopen( textfile, "r" );


      if ( tfile == NULL )
        fprintf( stderr, "could not read textfile '%s'\n", textfile );
      else
      {
        long   tsize;


        fseek( tfile, 0, SEEK_END );
        tsize = ftell( tfile );

        fseek( tfile, 0, SEEK_SET );
        text = (unsigned char*)malloc( tsize + 1 );

        if ( text != NULL )
        {
          fread( text, tsize, 1, tfile );
          text[tsize] = 0;
        }
        else
        {
          fprintf( stderr, "not enough memory to read '%s'\n", textfile );
          text = (unsigned char *)default_text;
        }

        fclose( tfile );
      }
    }

     
    if ( adisplay_init( adisplay, gr_pixel_mode_rgb24 ) < 0 )
    {
      fprintf( stderr, "could not initialize display!  Aborting.\n" );
      exit( 1 );
    }
    display->disp      = adisplay;
    display->disp_draw = adisplay_draw_glyph;
    display->disp_text = adisplay_draw_text;

    render_state_init( state, display );

    if ( resolution > 0 )
      render_state_set_resolution( state, resolution );

    if (size > 0.0)
      render_state_set_size( state, size );

    render_state_set_files( state, argv );
    render_state_set_file( state, 0 );

    grSetTitle( adisplay->surface, "FreeType Text Proofer, press F1 for help" );

    for (;;)
    {
      grEvent  event;

      adisplay_clear( adisplay );

      render_state_draw( state, text, 0,
                         10,                10, DIM_X / 3 - 15, DIM_Y - 70 );
      render_state_draw( state, text, 1,
                         DIM_X     / 3 + 5, 10, DIM_X / 3 - 15, DIM_Y - 70 );
      render_state_draw( state, text, 2,
                         DIM_X * 2 / 3 + 5, 10, DIM_X / 3 - 15, DIM_Y - 70 );

      write_message( state );
      grRefreshSurface( adisplay->surface );
      grListenSurface( adisplay->surface, 0, &event );
      if ( process_event( state, &event ) )
        break;
    }

    render_state_done( state );
    adisplay_done( adisplay );
    exit( 0 );   

    return 0;    
  }
