  event_render_mode_change( int delta )
  {
    if ( delta )
    {
      status.render_mode = ( status.render_mode + delta ) % N_RENDER_MODES;

      if ( status.render_mode < 0 )
        status.render_mode += N_RENDER_MODES;
    }

    switch ( status.render_mode )
    {
    case RENDER_MODE_STRING:
      status.header = NULL;
      break;

    case RENDER_MODE_KERNCMP:
      status.header = (char *)"Kerning comparison";
      break;
    }
  }
