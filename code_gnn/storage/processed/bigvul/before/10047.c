  gamma_ramp_draw( FT_Byte    gamma_ramp[256],
                   grBitmap*  bitmap )
  {
    int       i, x, y;
    FT_Byte*  p = (FT_Byte*)bitmap->buffer;

    if ( bitmap->pitch < 0 )
      p += -bitmap->pitch * ( bitmap->rows - 1 );

    x = ( bitmap->width - 256 ) / 2;
    y = ( bitmap->rows + 256 ) / 2;

    for (i = 0; i < 256; i++)
      p[bitmap->pitch * ( y - gamma_ramp[i] ) + ( x + i )] = 80;
  }
