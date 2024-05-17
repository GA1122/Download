  usage( void )
  {
    fprintf( stderr,
      "ftdiff: a simple program to proof several text hinting modes\n"
      "-----------------------------------------------------------\n"
      "\n"
      "Usage: ftdiff [options] fontfile [fontfile2 ...]\n"
      "\n"
      "  -r R         use resolution R dpi (default: 72 dpi)\n"
      "  -s S         set character size to S points (default: 16 pt)\n"
      "  -f TEXTFILE  change displayed text, using text in TEXTFILE\n"
      "\n" );
    exit( 1 );
  }
