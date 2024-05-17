  adisplay_init( ADisplay     display,
                 grPixelMode  mode )
  {
    grSurface*  surface;
    grBitmap    bit;


    if ( mode != gr_pixel_mode_gray  &&
         mode != gr_pixel_mode_rgb24 )
      return -1;

    grInitDevices();

    bit.mode  = mode;
    bit.width = DIM_X;
    bit.rows  = DIM_Y;
    bit.grays = 256;

    surface = grNewSurface( 0, &bit );

    if ( !surface )
      return -1;

    display->surface = surface;
    display->bitmap  = &surface->bitmap;
    display->gamma   = 1.0;

    grSetGlyphGamma( display->gamma );

    memset( &display->fore_color, 0, sizeof( grColor ) );
    memset( &display->back_color, 0xff, sizeof( grColor ) );

    return 0;
  }
