png_set_sCAL_s(png_structp png_ptr, png_infop info_ptr,
             int unit, png_charp swidth, png_charp sheight)
{
   png_uint_32 length;

   png_debug1(1, "in %s storage function", "sCAL");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   info_ptr->scal_unit = (png_byte)unit;

   length = png_strlen(swidth) + 1;
   png_debug1(3, "allocating unit for info (%u bytes)",
      (unsigned int)length);
   info_ptr->scal_s_width = (png_charp)png_malloc_warn(png_ptr, length);
   if (info_ptr->scal_s_width == NULL)
   {
      png_warning(png_ptr,
         "Memory allocation failed while processing sCAL.");
      return;
   }
   png_memcpy(info_ptr->scal_s_width, swidth, (png_size_t)length);

   length = png_strlen(sheight) + 1;
   png_debug1(3, "allocating unit for info (%u bytes)",
      (unsigned int)length);
   info_ptr->scal_s_height = (png_charp)png_malloc_warn(png_ptr, length);
   if (info_ptr->scal_s_height == NULL)
   {
      png_free (png_ptr, info_ptr->scal_s_width);
      info_ptr->scal_s_width = NULL;
      png_warning(png_ptr,
         "Memory allocation failed while processing sCAL.");
      return;
   }
   png_memcpy(info_ptr->scal_s_height, sheight, (png_size_t)length);
   info_ptr->valid |= PNG_INFO_sCAL;
#ifdef PNG_FREE_ME_SUPPORTED
   info_ptr->free_me |= PNG_FREE_SCAL;
#endif
}
