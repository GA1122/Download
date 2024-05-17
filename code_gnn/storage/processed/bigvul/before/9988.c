  adisplay_draw_glyph( void*        _display,
                       DisplayMode  mode,
                       int          x,
                       int          y,
                       int          width,
                       int          height,
                       int          pitch,
                       void*        buffer )
  {
    ADisplay  display = (ADisplay)_display;
    grBitmap  glyph;


    glyph.width  = width;
    glyph.rows   = height;
    glyph.pitch  = pitch;
    glyph.buffer = (unsigned char*)buffer;
    glyph.grays  = 256;
    glyph.mode   = gr_pixel_mode_mono;

    if ( mode == DISPLAY_MODE_GRAY )
      glyph.mode = gr_pixel_mode_gray;
    else if ( mode == DISPLAY_MODE_LCD )
      glyph.mode = gr_pixel_mode_lcd;

    grBlitGlyphToBitmap( display->bitmap, &glyph,
                         x, y, display->fore_color );
  }
