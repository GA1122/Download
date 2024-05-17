  event_gamma_change( double delta )
  {
    int i;
    double gamma_inv;

    status.gamma += delta;

    if ( status.gamma > 3.0 )
      status.gamma = 3.0;
    else if ( status.gamma < 0.1 )
      status.gamma = 0.1;

    sprintf( status.header_buffer, "gamma changed to %.1f", status.gamma );
    status.header = status.header_buffer;

    gamma_inv = 1.0f / status.gamma;

    for ( i = 0; i < 256; i++ )
      status.gamma_ramp[i] = (FT_Byte)( pow( (double)i / 255.0f, gamma_inv )
                                        * 255.0f );
  }
