  adisplay_draw_text( void*        _display,
                      int          x,
                      int          y,
                      const char*  msg )
  {
    ADisplay  adisplay = (ADisplay)_display;


    grWriteCellString( adisplay->bitmap, x, y, msg,
                       adisplay->fore_color );
  }
