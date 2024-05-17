png_get_pixels_per_meter(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
#ifdef PNG_pHYs_SUPPORTED
   if (info_ptr->valid & PNG_INFO_pHYs)
   {
      png_debug1(1, "in %s retrieval function", "png_get_pixels_per_meter");

      if (info_ptr->phys_unit_type != PNG_RESOLUTION_METER ||
         info_ptr->x_pixels_per_unit != info_ptr->y_pixels_per_unit)
          return (0);

      else
          return (info_ptr->x_pixels_per_unit);
   }
#else
   return (0);
#endif
   return (0);
}
