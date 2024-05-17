  parse_cmdline( int*    argc,
                 char**  argv[] )
  {
    char*  execname;
    int    option;


    execname = ft_basename( (*argv)[0] );

    while ( 1 )
    {
      option = getopt( *argc, *argv, "f:r:" );

      if ( option == -1 )
        break;

      switch ( option )
      {
      case 'f':
        status.Num  = atoi( optarg );
        break;

      case 'r':
        status.res = atoi( optarg );
        if ( status.res < 1 )
          usage( execname );
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

    status.ptsize = (int)(atof( *argv[0] ) * 64.0);
    if ( status.ptsize == 0 )
      status.ptsize = 64*10;

    if ( status.res <= 0 )
      status.res = 72;

    (*argc)--;
    (*argv)++;
  }