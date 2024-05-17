  render_state_set_size( RenderState  state,
                         float        char_size )
  {
    state->char_size    = char_size;
    state->need_rescale = 1;
  }
