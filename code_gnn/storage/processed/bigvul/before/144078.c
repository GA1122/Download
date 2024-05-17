png_set_gAMA_fixed(png_structp png_ptr, png_infop info_ptr, png_fixed_point
   int_gamma)
{
   png_fixed_point png_gamma;

   png_debug1(1, "in %s storage function", "gAMA");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   if (int_gamma > (png_fixed_point)PNG_UINT_31_MAX)
   {
      png_warning(png_ptr, "Limiting gamma to 21474.83");
      png_gamma=PNG_UINT_31_MAX;
   }
   else
   {
      if (int_gamma < 0)
      {
         png_warning(png_ptr, "Setting negative gamma to zero");
         png_gamma = 0;
      }
      else
         png_gamma = int_gamma;
   }
#ifdef PNG_FLOATING_POINT_SUPPORTED
   info_ptr->gamma = (float)(png_gamma/100000.);
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
   info_ptr->int_gamma = png_gamma;
#endif
   info_ptr->valid |= PNG_INFO_gAMA;
   if (png_gamma == 0)
      png_warning(png_ptr, "Setting gamma=0");
}
