  Process_Event( grEvent*  event )
  {
    int  ret = 0;


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
                           ? (char *)"now using embedded bitmaps (if available)"
                           : (char *)"now ignoring embedded bitmaps";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'c' ):
      handle->use_sbits_cache = !handle->use_sbits_cache;
      status.header           = handle->use_sbits_cache
                                 ? (char *)"now using sbits cache"
                                 : (char *)"now using normal cache";
      break;

    case grKEY( 'f' ):
      handle->autohint = !handle->autohint;
      status.header    = handle->autohint
                          ? (char *)"forced auto-hinting is now on"
                          : (char *)"forced auto-hinting is now off";

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( 'h' ):
      handle->hinted = !handle->hinted;
      status.header  = handle->hinted
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

    case grKEY( 'L' ):
      handle->lcd_mode = ( handle->lcd_mode + 1 ) % N_LCD_MODES;

      switch ( handle->lcd_mode )
      {
      case LCD_MODE_AA:
        status.header = (char *)"use normal anti-aliased rendering";
        break;
      case LCD_MODE_LIGHT:
        status.header = (char *)"use light anti-aliased rendering";
        break;
      case LCD_MODE_RGB:
        status.header = (char *)"use horizontal LCD-optimized rendering (RGB)";
        break;
      case LCD_MODE_BGR:
        status.header = (char *)"use horizontal LCD-optimized rendering (BGR)";
        break;
      case LCD_MODE_VRGB:
        status.header = (char *)"use vertical LCD-optimized rendering (RGB)";
        break;
      case LCD_MODE_VBGR:
        status.header = (char *)"use vertical LCD-optimized rendering (BGR)";
        break;
      }

      FTDemo_Update_Current_Flags( handle );
      break;

    case grKEY( ' ' ):
      event_render_mode_change( 1 );
      break;

    case grKEY( 'G' ):
      event_gamma_grid();
      break;

    case grKEY( 's' ):
      event_slant_change( 0.02 );
      break;

    case grKEY( 'S' ):
      event_slant_change( -0.02 );
      break;

    case grKEY( 'e' ):
      event_bold_change( 0.002 );
      break;

    case grKEY( 'E' ):
      event_bold_change( -0.002 );
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
    case grKeyPageUp:   event_size_change(  640 ); break;
    case grKeyPageDown: event_size_change( -640 ); break;

    case grKeyLeft:  event_index_change(    -1 ); break;
    case grKeyRight: event_index_change(     1 ); break;
    case grKeyF7:    event_index_change(   -10 ); break;
    case grKeyF8:    event_index_change(    10 ); break;
    case grKeyF9:    event_index_change(  -100 ); break;
    case grKeyF10:   event_index_change(   100 ); break;
    case grKeyF11:   event_index_change( -1000 ); break;
    case grKeyF12:   event_index_change(  1000 ); break;

    case grKEY( 'F' ):
      FTC_Manager_RemoveFaceID( handle->cache_manager,
                                handle->scaler.face_id );

      status.use_custom_lcd_filter = !status.use_custom_lcd_filter;
      if ( status.use_custom_lcd_filter )
        FT_Library_SetLcdFilterWeights( handle->library,
                                        status.filter_weights );
      else
        FT_Library_SetLcdFilterWeights( handle->library,
                                        (unsigned char*)"\x10\x40\x70\x40\x10" );
      status.header = status.use_custom_lcd_filter
                      ? (char *)"using custom LCD filter weights"
                      : (char *)"using default LCD filter";
      break;

    case grKEY( '[' ):
      if ( !status.use_custom_lcd_filter )
        break;

      status.fw_index--;
      if ( status.fw_index < 0 )
        status.fw_index = 4;
      break;

    case grKEY( ']' ):
      if ( !status.use_custom_lcd_filter )
        break;

      status.fw_index++;
      if ( status.fw_index > 4 )
        status.fw_index = 0;
      break;

    case grKEY( '-' ):
      if ( !status.use_custom_lcd_filter )
        break;

      FTC_Manager_RemoveFaceID( handle->cache_manager,
                                handle->scaler.face_id );

      status.filter_weights[status.fw_index]--;
      FT_Library_SetLcdFilterWeights( handle->library,
                                      status.filter_weights );
      break;

    case grKEY( '+' ):
    case grKEY( '=' ):
      if ( !status.use_custom_lcd_filter )
        break;

      FTC_Manager_RemoveFaceID( handle->cache_manager,
                                handle->scaler.face_id );

      status.filter_weights[status.fw_index]++;
      FT_Library_SetLcdFilterWeights( handle->library,
                                      status.filter_weights );
      break;

    default:
      break;
    }

    return ret;
  }
