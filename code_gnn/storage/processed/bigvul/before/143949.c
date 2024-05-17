png_get_bKGD(png_structp png_ptr, png_infop info_ptr,
   png_color_16p *background)
{
   if (png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_bKGD)
      && background != NULL)
   {
      png_debug1(1, "in %s retrieval function", "bKGD");

      *background = &(info_ptr->background);
      return (PNG_INFO_bKGD);
   }
   return (0);
}
