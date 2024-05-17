  render_state_init( RenderState  state,
                     Display      display )
  {
    memset( state, 0, sizeof ( *state ) );

    state->text         = default_text;
    state->filepath     = state->filepath0;
    state->filename     = "<none>";
    state->filepath0[0] = 0;
    state->resolution   = 72;
    state->char_size    = 16;
    state->display      = display[0];

    state->columns[0].use_kerning              = 1;
    state->columns[0].use_deltas               = 1;
    state->columns[0].use_lcd_filter           = 1;
    state->columns[0].use_global_advance_width = 1;
    state->columns[0].lcd_filter               = FT_LCD_FILTER_DEFAULT;
    state->columns[0].hint_mode                = HINT_MODE_BYTECODE;
    state->columns[0].use_custom_lcd_filter    = 0;
    state->columns[0].fw_index                 = 2;
     
    memcpy( state->columns[0].filter_weights, "\x10\x40\x70\x40\x10", 5);

    state->columns[1]                       = state->columns[0];
    state->columns[1].hint_mode             = HINT_MODE_AUTOHINT;
    state->columns[1].use_custom_lcd_filter = 1;

    state->columns[2]           = state->columns[0];
    state->columns[2].hint_mode = HINT_MODE_UNHINTED;

    state->col = 1;

    if ( FT_Init_FreeType( &state->library ) != 0 )
      panic( "could not initialize FreeType library. Check your code\n" );
  }
