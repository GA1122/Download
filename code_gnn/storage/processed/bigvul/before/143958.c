png_get_gAMA(png_structp png_ptr, png_infop info_ptr, double *file_gamma)
{
   png_debug1(1, "in %s retrieval function", "gAMA");

   if (png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA)
      && file_gamma != NULL)
   {
      *file_gamma = (double)info_ptr->gamma;
      return (PNG_INFO_gAMA);
   }
   return (0);
}
