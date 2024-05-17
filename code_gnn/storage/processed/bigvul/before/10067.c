  event_slant_change( double  delta )
  {
    status.slant += delta;

    if ( status.slant > 1.0 )
      status.slant = 1.0;
    else if ( status.slant < -1.0 )
      status.slant = -1.0;

    sprintf( status.header_buffer, "slanting changed to %.3f",
             status.slant );

    status.header = status.header_buffer;
  }
