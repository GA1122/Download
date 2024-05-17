  parse_cmdline( int*    argc,
                 char**  argv[] )
  {
    char*  execname;
    int    option;


    execname = ft_basename( (*argv)[0] );

    while ( 1 )
    {
      option = getopt( *argc, *argv, "Dde:f:L:l:r:m:p" );

      if ( option == -1 )
        break;

      switch ( option )
      {
      case 'd':
        status.debug = 1;
        break;

      case 'D':
        status.dump_cache_stats = 1;
        break;

      case 'e':
        status.encoding = FTDemo_Make_Encoding_Tag( optarg );
        break;

      case 'f':
        status.Num  = atoi( optarg );
        break;

      case 'L':
        status.trace_level = atoi( optarg );
        if ( status.trace_level < 1 || status.trace_level > 7 )
          usage( execname );
        break;

      case 'l':
        status.lcd_mode = atoi( optarg );
        if ( status.lcd_mode < 0 || status.lcd_mode > N_LCD_MODES )
        {
          fprintf( stderr, "argument to `l' must be between 0 and %d\n",
                   N_LCD_MODES );
          exit( 3 );
        }
        break;

      case 'm':
        Text               = (unsigned char*)optarg;
        status.render_mode = RENDER_MODE_TEXT;
        break;

      case 'r':
        status.res = atoi( optarg );
        if ( status.res < 1 )
          usage( execname );
        break;

      case 'p':
        status.preload = 1;
        break;

      default:
        usage( execname );
        break;
      }
    }

    *argc -= optind;
    *argv += optind;

    if ( *argc <= 1 )
      usage( execname );

    status.ptsize = (int)( atof( *argv[0] ) * 64.0 );
    if ( status.ptsize == 0 )
      status.ptsize = 64 * 10;

    (*argc)--;
    (*argv)++;
  }
