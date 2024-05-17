png_get_pixel_aspect_ratio(png_structp png_ptr, png_infop info_ptr)
   {
   if (png_ptr != NULL && info_ptr != NULL)
#ifdef PNG_pHYs_SUPPORTED

   if (info_ptr->valid & PNG_INFO_pHYs)
   {
      png_debug1(1, "in %s retrieval function", "png_get_aspect_ratio");

      if (info_ptr->x_pixels_per_unit == 0)
         return ((float)0.0);

      else
         return ((float)((float)info_ptr->y_pixels_per_unit
            /(float)info_ptr->x_pixels_per_unit));
   }
#else
      return (0.0);
#endif
   return ((float)0.0);
}
