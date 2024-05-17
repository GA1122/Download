  event_help( void )
  {
    grEvent  dummy_event;


    FTDemo_Display_Clear( display );
    grGotoxy( 0, 0 );
    grSetMargin( 2, 1 );
    grGotobitmap( display->bitmap );

    grWriteln( "FreeType Glyph Viewer - part of the FreeType test suite" );
    grLn();
    grWriteln( "This program is used to display all glyphs from one or" );
    grWriteln( "several font files, with the FreeType library." );
    grLn();
    grWriteln( "Use the following keys:" );
    grLn();
    grWriteln( "  F1 or ?    : display this help screen" );
    grLn();
    grWriteln( "  a          : toggle anti-aliasing" );
    grWriteln( "  left/right : decrement/increment glyph index" );
    grWriteln( "  up/down    : change character size" );
    grLn();
    grWriteln( "  F7         : decrement index by 10" );
    grWriteln( "  F8         : increment index by 10" );
    grWriteln( "  F9         : decrement index by 100" );
    grWriteln( "  F10        : increment index by 100" );
    grWriteln( "  F11        : decrement index by 1000" );
    grWriteln( "  F12        : increment index by 1000" );
    grLn();
    grWriteln( "  i          : move grid up" );
    grWriteln( "  j          : move grid left" );
    grWriteln( "  k          : move grid down" );
    grWriteln( "  l          : move grid right" );
    grWriteln( "  Page up/dn : zoom in/out grid" );
    grWriteln( "  RETURN     : reset zoom and position" );
    grLn();
    grWriteln( "  H          : toggle horizontal hinting" );
    grWriteln( "  V          : toggle vertical hinting" );
    grWriteln( "  B          : toggle blue zone hinting" );
    grWriteln( "  d          : toggle dots display" );
    grWriteln( "  o          : toggle outline display" );
    grWriteln( "  g          : increase gamma by 0.1" );
    grWriteln( "  v          : decrease gamma by 0.1" );
    grLn();
    grWriteln( "  n          : next font" );
    grWriteln( "  p          : previous font" );
    grWriteln( "  q / ESC    : exit program" );
    grLn();
    grWriteln( "  1          : dump edge hints" );
    grWriteln( "  2          : dump segment hints" );
    grWriteln( "  3          : dump point hints" );
    grLn();
    grWriteln( "press any key to exit this help screen" );

    grRefreshSurface( display->surface );
    grListenSurface( display->surface, gr_event_key, &dummy_event );
  }
