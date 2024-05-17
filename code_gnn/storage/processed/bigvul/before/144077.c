png_set_gAMA(png_structp png_ptr, png_infop info_ptr, double file_gamma)
{
   double png_gamma;

   png_debug1(1, "in %s storage function", "gAMA");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

    
   if (file_gamma > 21474.83)
   {
      png_warning(png_ptr, "Limiting gamma to 21474.83");
      png_gamma=21474.83;
   }
   else
      png_gamma = file_gamma;
   info_ptr->gamma = (float)png_gamma;
#ifdef PNG_FIXED_POINT_SUPPORTED
   info_ptr->int_gamma = (int)(png_gamma*100000.+.5);
#endif
   info_ptr->valid |= PNG_INFO_gAMA;
   if (png_gamma == 0.0)
      png_warning(png_ptr, "Setting gamma=0");
}
