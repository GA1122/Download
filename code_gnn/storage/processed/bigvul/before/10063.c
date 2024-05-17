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
    grWriteln( "  F1, ?       display this help screen" );
    grLn();
    grWriteln( "  a           toggle anti-aliasing" );
    grWriteln( "  b           toggle embedded bitmaps" );
    grWriteln( "  c           toggle between cache modes" );
    grWriteln( "  f           toggle forced auto-hinting" );
    grWriteln( "  h           toggle outline hinting" );
    grWriteln( "  l           toggle low precision rendering" );
    grLn();
    grWriteln( "  L           cycle through LCD modes" );
    grWriteln( "  space       cycle through rendering modes" );
    grWriteln( "  1-6         select rendering mode" );
    grLn();
    grWriteln( "  e, E        adjust emboldening" );
    grWriteln( "  s, S        adjust slanting" );
    grLn();
    grWriteln( "  F           toggle custom LCD filter mode" );
    grWriteln( "  [, ]        select custom LCD filter weight" );
    grWriteln( "  -, +(=)     adjust selected custom LCD filter weight" );
    grLn();
    grWriteln( "  G           show gamma ramp" );
    grWriteln( "  g, v        adjust gamma value" );
    grLn();
    grWriteln( "  p, n        select previous/next font" );
    grLn();
    grWriteln( "  Up, Down    adjust pointsize by 1 unit" );
    grWriteln( "  PgUp, PgDn  adjust pointsize by 10 units" );
    grLn();
    grWriteln( "  Left, Right adjust index by 1" );
    grWriteln( "  F7, F8      adjust index by 10" );
    grWriteln( "  F9, F10     adjust index by 100" );
    grWriteln( "  F11, F12    adjust index by 1000" );
    grLn();
    grWriteln( "press any key to exit this help screen" );

    grRefreshSurface( display->surface );
    grListenSurface( display->surface, gr_event_key, &dummy_event );
  }
