  event_change_size( RenderState  state,
                     double       delta )
  {
    double  char_size = state->char_size;


    char_size += delta;
    if ( char_size < 6.0 )
      char_size = 6.0;
    else if ( char_size > 300.0 )
      char_size = 300.0;

    render_state_set_size( state, char_size );
  }
