  usage( char*  execname )
  {
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "ftmulti: multiple masters font viewer - part of FreeType\n" );
    fprintf( stderr,  "--------------------------------------------------------\n" );
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "Usage: %s [options below] ppem fontname[.pfb|.ttf] ...\n",
             execname );
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "  -e encoding  select encoding (default: no encoding)\n" );
    fprintf( stderr,  "  -r R         use resolution R dpi (default: 72 dpi)\n" );
    fprintf( stderr,  "  -f index     specify first glyph index to display\n" );
    fprintf( stderr,  "  -d \"axis1 axis2 ...\"\n"
                      "               specify the design coordinates for each axis\n" );
    fprintf( stderr,  "\n" );

    exit( 1 );
  }
