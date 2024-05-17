  adisplay_clear( ADisplay  display )
  {
    grBitmap*  bit   = display->bitmap;
    int        pitch = bit->pitch;


    if ( pitch < 0 )
      pitch = -pitch;

    if ( bit->mode == gr_pixel_mode_gray )
      memset( bit->buffer, display->back_color.value, pitch * bit->rows );
    else
    {
      unsigned char*  p = bit->buffer;
      int             i, j;


      for ( i = 0; i < bit->rows; i++ )
      {
        for ( j = 0; j < bit->width; j++ )
          memcpy( p + 3 * j, display->back_color.chroma, 3 );

        p += pitch;
      }
    }
  }
