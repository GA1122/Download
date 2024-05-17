  render_state_done( RenderState  state )
  {
    if ( state->filepath != state->filepath0 )
    {
      free( (char*)state->filepath );
      state->filepath = state->filepath0;
    }
    state->filepath0[0] = 0;
    state->filename     = 0;

    if ( state->face )
    {
      FT_Done_Face( state->face );
      state->face = NULL;
      state->size = NULL;
    }

    if ( state->library )
    {
      FT_Done_FreeType( state->library );
      state->library = NULL;
    }
  }
