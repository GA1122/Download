png_set_cHRM(png_structp png_ptr, png_infop info_ptr,
   double white_x, double white_y, double red_x, double red_y,
   double green_x, double green_y, double blue_x, double blue_y)
{
   png_debug1(1, "in %s storage function", "cHRM");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   info_ptr->x_white = (float)white_x;
   info_ptr->y_white = (float)white_y;
   info_ptr->x_red   = (float)red_x;
   info_ptr->y_red   = (float)red_y;
   info_ptr->x_green = (float)green_x;
   info_ptr->y_green = (float)green_y;
   info_ptr->x_blue  = (float)blue_x;
   info_ptr->y_blue  = (float)blue_y;
#ifdef PNG_FIXED_POINT_SUPPORTED
   info_ptr->int_x_white = (png_fixed_point)(white_x*100000.+0.5);
   info_ptr->int_y_white = (png_fixed_point)(white_y*100000.+0.5);
   info_ptr->int_x_red   = (png_fixed_point)(  red_x*100000.+0.5);
   info_ptr->int_y_red   = (png_fixed_point)(  red_y*100000.+0.5);
   info_ptr->int_x_green = (png_fixed_point)(green_x*100000.+0.5);
   info_ptr->int_y_green = (png_fixed_point)(green_y*100000.+0.5);
   info_ptr->int_x_blue  = (png_fixed_point)( blue_x*100000.+0.5);
   info_ptr->int_y_blue  = (png_fixed_point)( blue_y*100000.+0.5);
#endif
   info_ptr->valid |= PNG_INFO_cHRM;
}
