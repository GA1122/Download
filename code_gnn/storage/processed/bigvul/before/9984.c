  _render_state_rescale( RenderState  state )
  {
    if ( state->need_rescale && state->size )
    {
      FT_Set_Char_Size( state->face, 0,
                        (FT_F26Dot6)( state->char_size * 64.0 ),
                        0, state->resolution );
      state->need_rescale = 0;
    }
  }
