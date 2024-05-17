  Process_Event( grEvent*  event )
  {
    FTDemo_String_Context*  sc = &status.sc;
    int                     ret = 0;


    if ( event->key >= '1' && event->key < '1' + N_RENDER_MODES )
    {
      status.render_mode = event->key - '1';
      event_render_mode_change( 0 );

      return ret;
    }

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
      status.header     = handle->antialias
                          ? (char *)"anti-aliasing is now on"
                          : (char *)"anti-aliasing is now off";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'b' ):
      handle->use_sbits = !handle->use_sbits;
      status.header     = handle->use_sbits
                          ? (char *)"embedded bitmaps are now used when available"
                          : (char *)"embedded bitmaps are now ignored";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'f' ):
      handle->autohint = !handle->autohint;
      status.header     = handle->autohint
                          ? (char *)"forced auto-hinting is now on"
                          : (char *)"forced auto-hinting is now off";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'h' ):
      handle->hinted = !handle->hinted;
      status.header   = handle->hinted
                        ? (char *)"glyph hinting is now active"
                        : (char *)"glyph hinting is now ignored";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'l' ):
      handle->low_prec = !handle->low_prec;
      status.header    = handle->low_prec
                         ? (char *)"rendering precision is now forced to low"
                         : (char *)"rendering precision is now normal";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'k' ):
      sc->kerning_mode = ( sc->kerning_mode + 1 ) % N_KERNING_MODES;
      status.header =
        sc->kerning_mode == KERNING_MODE_SMART
        ? (char *)"pair kerning and side bearing correction is now active"
        : sc->kerning_mode == KERNING_MODE_NORMAL
          ? (char *)"pair kerning is now active"
          : (char *)"pair kerning is now ignored";
      break;

    case grKEY( 't' ):
      sc->kerning_degree = ( sc->kerning_degree + 1 ) % N_KERNING_DEGREES;
      status.header =
        sc->kerning_degree == KERNING_DEGREE_NONE
        ? (char *)"no track kerning"
        : sc->kerning_degree == KERNING_DEGREE_LIGHT
          ? (char *)"light track kerning active"
          : sc->kerning_degree == KERNING_DEGREE_MEDIUM
            ? (char *)"medium track kerning active"
            : (char *)"tight track kerning active";
      break;

    case grKEY( 'V' ):
      sc->vertical  = !sc->vertical;
      status.header = sc->vertical
                      ? (char *)"using vertical layout"
                      : (char *)"using horizontal layout";
      break;

    case grKEY( 'G' ):
      sc->gamma_ramp = sc->gamma_ramp ? NULL : status.gamma_ramp;
      status.header  = sc->gamma_ramp
                       ? (char *)"gamma correction is now on"
                       : (char *)"gamma correction is now off";
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

    case grKEY( 'p' ):
      event_font_change( -1 );
      break;

    case grKeyUp:       event_size_change(   64 ); break;
    case grKeyDown:     event_size_change(  -64 ); break;
    case grKeyPageUp:   event_size_change(  640); break;
    case grKeyPageDown: event_size_change( -640 ); break;

    case grKeyLeft:  event_angle_change(    -3 ); break;
    case grKeyRight: event_angle_change(     3 ); break;
    case grKeyF7:    event_angle_change(   -30 ); break;
    case grKeyF8:    event_angle_change(    30 ); break;

    default:
      break;
    }

    return ret;
  }
