png_get_cHRM_fixed(png_structp png_ptr, png_infop info_ptr,
   png_fixed_point *white_x, png_fixed_point *white_y, png_fixed_point *red_x,
   png_fixed_point *red_y, png_fixed_point *green_x, png_fixed_point *green_y,
   png_fixed_point *blue_x, png_fixed_point *blue_y)
{
   png_debug1(1, "in %s retrieval function", "cHRM");

   if (png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
   {
      if (white_x != NULL)
         *white_x = info_ptr->int_x_white;
      if (white_y != NULL)
         *white_y = info_ptr->int_y_white;
      if (red_x != NULL)
         *red_x = info_ptr->int_x_red;
      if (red_y != NULL)
         *red_y = info_ptr->int_y_red;
      if (green_x != NULL)
         *green_x = info_ptr->int_x_green;
      if (green_y != NULL)
         *green_y = info_ptr->int_y_green;
      if (blue_x != NULL)
         *blue_x = info_ptr->int_x_blue;
      if (blue_y != NULL)
         *blue_y = info_ptr->int_y_blue;
      return (PNG_INFO_cHRM);
   }
   return (0);
}
