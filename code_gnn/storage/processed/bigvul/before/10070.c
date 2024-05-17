  usage( char*  execname )
  {
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "ftview: simple glyph viewer -- part of the FreeType project\n" );
    fprintf( stderr,  "-----------------------------------------------------------\n" );
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "Usage: %s [options] pt font ...\n",
             execname );
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "  pt        The point size for the given resolution.\n" );
    fprintf( stderr,  "            If resolution = 72 dpi, this directly gives the\n" );
    fprintf( stderr,  "            ppem value (pixels per EM).\n" );
    fprintf( stderr,  "  font      The font file(s) to display; if no extension is given,\n" );
    fprintf( stderr,  "            ftview tries to append one of the following suffixes:\n" );
    fprintf( stderr,  "            `.ttf', `.ttc', `.otf', `.pfa', `.pfb'.\n" );
    fprintf( stderr,  "            For `.pfa' and `.pfb' files, ftview also tries to\n" );
    fprintf( stderr,  "            attach the corresponding metrics file (with extension\n" );
    fprintf( stderr,  "            `.afm' or `.pfm').\n" );
    fprintf( stderr,  "\n" );
    fprintf( stderr,  "  -r R      Use resolution R dpi (default: 72 dpi).\n" );
    fprintf( stderr,  "  -f index  Specify first index to display.\n" );
    fprintf( stderr,  "  -e enc    Specify encoding tag (default: no encoding).\n" );
    fprintf( stderr,  "  -D        Dump cache usage statistics.\n" );
    fprintf( stderr,  "  -m text   Use `text' for rendering.\n" );
    fprintf( stderr,  "  -l nn     Change rendering mode (0 <= nn <= %d).\n",
             N_LCD_MODES );
    fprintf( stderr,  "  -p        Preload file in memory to simulate memory-mapping.\n" );
    fprintf( stderr,  "\n" );

    exit( 1 );
  }
