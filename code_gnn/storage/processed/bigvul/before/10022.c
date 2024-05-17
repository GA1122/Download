grid_status_init( GridStatus       st,
                  FTDemo_Display*  display )
{
  st->scale         = 1.0;
  st->x_origin      = display->bitmap->width / 4;
  st->y_origin      = display->bitmap->rows / 4;
  st->margin        = 0.05;
  st->axis_color    = grFindColor( display->bitmap,   0,   0,   0, 255 );
  st->grid_color    = grFindColor( display->bitmap, 192, 192, 192, 255 );
  st->outline_color = grFindColor( display->bitmap, 255,   0,   0, 255 );
  st->on_color      = grFindColor( display->bitmap,  64,  64, 255, 255 );
  st->conic_color   = grFindColor( display->bitmap,   0, 128,   0, 255 );
  st->cubic_color   = grFindColor( display->bitmap, 255,  64, 255, 255 );
  st->disp_width    = display->bitmap->width;
  st->disp_height   = display->bitmap->rows;
  st->disp_bitmap   = display->bitmap;

  st->do_horz_hints = 1;
  st->do_vert_hints = 1;
  st->do_blue_hints = 1;
  st->do_dots       = 1;
  st->do_outline    = 1;

  st->Num    = 0;
  st->gamma  = 1.0;
  st->header = "";
}
