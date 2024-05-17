  Process_Event( grEvent*  event )
  {
    int  ret = 0;

    status.header = NULL;

    switch ( event->key )
    {
    case grKeyEsc:
    case grKEY( 'q' ):
      ret = 1;
      break;

    case grKeyF1:
    case grKEY( '?' ):
      event_help();
      break;

    case grKEY( 'a' ):
      handle->antialias = !handle->antialias;
      status.header     = handle->antialias ? "anti-aliasing is now on"
                                            : "anti-aliasing is now off";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( '1' ):
      af_glyph_hints_dump_edges( _af_debug_hints );
      break;

    case grKEY( '2' ):
      af_glyph_hints_dump_segments( _af_debug_hints );
      break;

    case grKEY( '3' ):
      af_glyph_hints_dump_points( _af_debug_hints );
      break;


    case grKEY( 'g' ):
      event_gamma_change( 0.1 );
      break;

    case grKEY( 'v' ):
      event_gamma_change( -0.1 );
      break;

    case grKEY( 'n' ):
      event_font_change( 1 );
      break;

    case grKEY( 'h' ):
      handle->hinted = !handle->hinted;
      status.header  = handle->hinted ? "glyph hinting is now active"
                                      : "glyph hinting is now ignored";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'd' ):
      status.do_dots = !status.do_dots;
      break;

    case grKEY( 'o' ):
      status.do_outline = !status.do_outline;
      break;

    case grKEY( 'p' ):
      event_font_change( -1 );
      break;

    case grKEY('H'):
      status.do_horz_hints = !status.do_horz_hints;
      status.header = status.do_horz_hints ? "horizontal hinting enabled"
                                           : "horizontal hinting disabled";
      break;

    case grKEY('V'):
      status.do_vert_hints = !status.do_vert_hints;
      status.header        = status.do_vert_hints
                             ? "vertical hinting enabled"
                             : "vertical hinting disabled";
      break;

    case grKEY('B'):
      status.do_blue_hints = !status.do_blue_hints;
      status.header        = status.do_blue_hints
                             ? "blue zone hinting enabled"
                             : "blue zone hinting disabled";
      break;


    case grKeyLeft:     event_index_change( -1 ); break;
    case grKeyRight:    event_index_change( +1 ); break;
    case grKeyF7:       event_index_change(   -10 ); break;
    case grKeyF8:       event_index_change(    10 ); break;
    case grKeyF9:       event_index_change(  -100 ); break;
    case grKeyF10:      event_index_change(   100 ); break;
    case grKeyF11:      event_index_change( -1000 ); break;
    case grKeyF12:      event_index_change(  1000 ); break;

    case grKeyUp:       event_size_change( +32 ); break;
    case grKeyDown:     event_size_change( -32 ); break;

    case grKEY( ' ' ):  event_grid_reset( &status );
                        status.do_horz_hints = 1;
                        status.do_vert_hints = 1;
                        break;

    case grKEY('i'):    event_grid_translate( 0, +1 ); break;
    case grKEY('k'):    event_grid_translate( 0, -1 ); break;
    case grKEY('l'):    event_grid_translate( +1, 0 ); break;
    case grKEY('j'):    event_grid_translate( -1, 0 ); break;

    case grKeyPageUp:   event_grid_zoom( 1.25 ); break;
    case grKeyPageDown: event_grid_zoom( 1/1.25 ); break;

    default:
      ;
    }

    return ret;
  }
