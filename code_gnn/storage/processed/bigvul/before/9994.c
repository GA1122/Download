  get_option_arg( char*   option,
                  char** *pargv,
                  char**  argend )
  {
    if ( option[2] == 0 )
    {
      char**  argv = *pargv;


      if ( ++argv >= argend )
        usage();
      option = argv[0];
      *pargv = argv;
    }
    else
      option += 2;

    return option;
  }
