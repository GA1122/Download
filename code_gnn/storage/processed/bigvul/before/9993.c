  event_help( RenderState  state )
  {
    ADisplay  display = (ADisplay)state->display.disp;
    grEvent   dummy_event;


    adisplay_clear( display );
    grGotoxy( 0, 0 );
    grSetMargin( 2, 1 );
    grGotobitmap( display->bitmap );

    grWriteln( "Text Viewer - Simple text/font proofer for the FreeType project" );
    grLn();
    grWriteln( "This program is used to display text using two distinct algorithms." );
    grWriteln( "On the left, text is rendered by the TrueType bytecode interpreter." );
    grWriteln( "In the middle, text is rendered through the FreeType auto-hinter." );
    grWriteln( "On the right, text is rendered unhinted." );
    grLn();
    grWriteln( "Use the following keys:" );
    grLn();
    grWriteln( "  F1, ?       display this help screen" );
    grLn();
    grWriteln( "  n, p        select previous/next font" );
    grLn();
    grWriteln( "  1, 2, 3     select left, middle, or right column" );
    grWriteln( "  a           toggle `ignore global advance width flag'" );
    grWriteln( "  d           toggle lsb/rsb deltas" );
    grWriteln( "  h           toggle hinting mode" );
    grWriteln( "  k           toggle kerning" );
    grWriteln( "  g, v        adjust gamma value" );
    grWriteln( "  r           toggle rendering mode" );
    grLn();
    grWriteln( "  l           change LCD filter type" );
    grWriteln( "  [, ]        select custom LCD filter weight" );
    grWriteln( "  -, +(=)     adjust selected custom LCD filter weight");
    grLn();
    grWriteln( "  Up, Down    adjust pointsize by 0.5 unit" );
    grWriteln( "  PgUp, PgDn  adjust pointsize by 5 units" );
    grLn();
    grWriteln( "press any key to exit this help screen" );

    grRefreshSurface( display->surface );
    grListenSurface( display->surface, gr_event_key, &dummy_event );
  }
