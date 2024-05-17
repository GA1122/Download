  event_gamma_change( double  delta )
  {
    status.gamma += delta;

    if ( status.gamma > 3.0 )
      status.gamma = 3.0;
    else if ( status.gamma < 0.0 )
      status.gamma = 0.0;

    grSetGlyphGamma( status.gamma );

    sprintf( status.header_buffer, "gamma changed to %.1f%s",
             status.gamma, status.gamma == 0.0 ? " (sRGB mode)" : "" );

    status.header = status.header_buffer;
  }
