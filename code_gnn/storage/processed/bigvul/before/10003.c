  render_state_set_resolution( RenderState  state,
                               int          resolution )
  {
    state->resolution   = resolution;
    state->need_rescale = 1;
  }
