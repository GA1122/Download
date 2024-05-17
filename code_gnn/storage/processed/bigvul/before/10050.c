  usage( char*  execname )
  {
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "ftstring: string viewer -- part of the FreeType project\n" );
    fprintf( stderr,  "-------------------------------------------------------\n" );
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "Usage: %s [options below] ppem fontname[.ttf|.ttc] ...\n",
             execname );
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "  -e enc      specify encoding tag (default: unic)\n" );
    fprintf( stderr,  "  -r R        use resolution R dpi (default: 72 dpi)\n" );
    fprintf( stderr,  "  -m message  message to display\n" );
    fprintf( stderr,  "\n" );

    exit( 1 );
  }
