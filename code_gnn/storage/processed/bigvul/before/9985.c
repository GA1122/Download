  adisplay_change_gamma( ADisplay  display,
                         double    delta )
  {
    display->gamma += delta;
    if ( display->gamma > 3.0 )
      display->gamma = 3.0;
    else if ( display->gamma < 0.0 )
      display->gamma = 0.0;

    grSetGlyphGamma( display->gamma );
  }
