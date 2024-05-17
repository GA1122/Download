png_set_rgb_to_gray_fixed(png_structp png_ptr, int error_action,
   png_fixed_point red, png_fixed_point green)
{
   png_debug(1, "in png_set_rgb_to_gray");

   if (png_ptr == NULL)
      return;

   switch(error_action)
   {
      case 1: png_ptr->transformations |= PNG_RGB_TO_GRAY;
              break;

      case 2: png_ptr->transformations |= PNG_RGB_TO_GRAY_WARN;
              break;

      case 3: png_ptr->transformations |= PNG_RGB_TO_GRAY_ERR;
   }
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#ifdef PNG_READ_EXPAND_SUPPORTED
      png_ptr->transformations |= PNG_EXPAND;
#else
   {
      png_warning(png_ptr,
        "Cannot do RGB_TO_GRAY without EXPAND_SUPPORTED.");
      png_ptr->transformations &= ~PNG_RGB_TO_GRAY;
   }
#endif
   {
      png_uint_16 red_int, green_int;
      if (red < 0 || green < 0)
      {
         red_int   =  6968;  
         green_int = 23434;  
      }
      else if (red + green < 100000L)
      {
         red_int = (png_uint_16)(((png_uint_32)red*32768L)/100000L);
         green_int = (png_uint_16)(((png_uint_32)green*32768L)/100000L);
      }
      else
      {
         png_warning(png_ptr, "ignoring out of range rgb_to_gray coefficients");
         red_int   =  6968;
         green_int = 23434;
      }
      png_ptr->rgb_to_gray_red_coeff   = red_int;
      png_ptr->rgb_to_gray_green_coeff = green_int;
      png_ptr->rgb_to_gray_blue_coeff  =
         (png_uint_16)(32768 - red_int - green_int);
   }
}
