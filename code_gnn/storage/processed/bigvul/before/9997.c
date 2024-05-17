  process_event( RenderState  state,
                 grEvent*     event )
  {
    int          ret    = 0;
    ColumnState  column = &state->columns[state->col];


    switch ( event->key )
    {
    case grKeyEsc:
    case grKEY( 'q' ):
      ret = 1;
      break;

    case grKeyF1:
    case grKEY( '?' ):
      event_help( state );
      break;

    case grKeyLeft:
      if ( --state->col < 0 )
        state->col = 2;
      state->message = state->message0;
      sprintf( state->message0, "column %d selected", state->col + 1 );
      break;

    case grKeyRight:
      if ( ++state->col > 2 )
        state->col = 0;
      state->message = state->message0;
      sprintf( state->message0, "column %d selected", state->col + 1 );
      break;

    case grKeyUp:
      event_change_size( state, +0.5 );
      break;

    case grKeyDown:
      event_change_size( state, -0.5 );
      break;

    case grKeyPageUp:
      event_change_size( state, +5. );
      break;

    case grKeyPageDown:
      event_change_size( state, -5. );
      break;

    case grKEY( '1' ):
      state->col     = 0;
      state->message = (char *)"column 1 selected";
      break;

    case grKEY( '2' ):
      state->col     = 1;
      state->message = (char *)"column 2 selected";
      break;

    case grKEY( '3' ):
      state->col     = 2;
      state->message = (char *)"column 3 selected";
      break;

    case grKEY( 'a' ):
      column->use_global_advance_width
                     = !column->use_global_advance_width;
      state->message = column->use_global_advance_width
                         ? (char *)"using global advance width"
                         : (char *)"ignoring global advance width";
      break;

    case grKEY( 'd' ):
      column->use_deltas = !column->use_deltas;
      state->message     = column->use_deltas
                             ? (char *)"using rsb/lsb deltas"
                             : (char *)"ignoring rsb/lsb deltas";
      break;

    case grKEY( 'g' ):
      event_change_gamma( state, +0.1 );
      break;

    case grKEY( 'h' ):
      column->hint_mode =
        (HintMode)( ( column->hint_mode + 1 ) % HINT_MODE_MAX );
      state->message = state->message0;
      sprintf( state->message0, "column %d is %s",
               state->col + 1, render_mode_names[column->hint_mode] );
      break;

    case grKEY( 'k' ):
      column->use_kerning = !column->use_kerning;
      state->message      = column->use_kerning
                              ? (char *)"using kerning"
                              : (char *)"ignoring kerning";
      break;

    case grKEY( 'l' ):
      switch ( column->lcd_filter )
      {
      case FT_LCD_FILTER_NONE:
        column->lcd_filter = FT_LCD_FILTER_DEFAULT;
        state->message     = (char *)"using default LCD filter";
        break;

      case FT_LCD_FILTER_DEFAULT:
        if ( !column->use_custom_lcd_filter )
        {
          column->use_custom_lcd_filter = 1;
          state->message                = (char*)"using custom LCD filter";
        }
        else
        {
          column->use_custom_lcd_filter = 0;
          column->lcd_filter            = FT_LCD_FILTER_LIGHT;
          state->message                = (char *)"using light LCD filter";
        }
        break;

      case FT_LCD_FILTER_LIGHT:
        column->lcd_filter = FT_LCD_FILTER_LEGACY;
        state->message     = (char *)"using legacy LCD filter";
        break;

      case FT_LCD_FILTER_LEGACY:
        column->lcd_filter = FT_LCD_FILTER_NONE;
        state->message     = (char *)"using no LCD filter";
        break;

      default:   
        break;
      }
      break;

    case grKEY( 'n' ):
      render_state_set_file( state, state->face_index + 1 );
      break;

    case grKEY( 'p' ):
      render_state_set_file( state, state->face_index - 1 );
      break;

    case grKEY( 'r' ):
      column->use_lcd_filter = !column->use_lcd_filter;
      state->message         = state->message0;
      sprintf( state->message0, "column %d is using %s",
               state->col + 1, column->use_lcd_filter ? "LCD filtering"
                                                      : "gray rendering" );
      break;

    case grKEY( 'v' ):
      event_change_gamma( state, -0.1 );
      break;

    case grKEY( '[' ):
      if ( !column->use_custom_lcd_filter )
        break;

      column->fw_index--;
      if ( column->fw_index < 0 )
        column->fw_index = 4;
      break;

    case grKEY( ']' ):
      if ( !column->use_custom_lcd_filter )
        break;

      column->fw_index++;
      if ( column->fw_index > 4 )
        column->fw_index = 0;
      break;

    case grKEY( '-' ):
      if ( !column->use_custom_lcd_filter )
        break;

      column->filter_weights[column->fw_index]--;
      break;

    case grKEY( '+' ):
    case grKEY( '=' ):
      if ( !column->use_custom_lcd_filter )
        break;

      column->filter_weights[column->fw_index]++;
      break;

    default:
      break;
    }

    return ret;
  }
