  event_change_gamma( RenderState  state,
                      double       delta )
  {
    ADisplay  display = (ADisplay)state->display.disp;


    adisplay_change_gamma( display, delta );
    if ( display->gamma == 0.0 )
      sprintf( state->message0, "gamma set to sRGB" );
    else
      sprintf( state->message0, "gamma set to %.1f", display->gamma );

    state->message = state->message0;
  }
