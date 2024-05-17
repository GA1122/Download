png_set_tRNS(png_structp png_ptr, png_infop info_ptr,
   png_bytep trans, int num_trans, png_color_16p trans_values)
{
   png_debug1(1, "in %s storage function", "tRNS");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   if (num_trans < 0 || num_trans > PNG_MAX_PALETTE_LENGTH)
      {
        png_warning(png_ptr, "Ignoring invalid num_trans value");
        return;
      }

   if (trans != NULL)
   {
        

#ifdef PNG_FREE_ME_SUPPORTED
       png_free_data(png_ptr, info_ptr, PNG_FREE_TRNS, 0);
#endif

        
       png_ptr->trans = info_ptr->trans = (png_bytep)png_malloc(png_ptr,
           (png_uint_32)PNG_MAX_PALETTE_LENGTH);
       if (num_trans > 0 && num_trans <= PNG_MAX_PALETTE_LENGTH)
          png_memcpy(info_ptr->trans, trans, (png_size_t)num_trans);
   }

   if (trans_values != NULL)
   {
      int sample_max = (1 << info_ptr->bit_depth);
      if ((info_ptr->color_type == PNG_COLOR_TYPE_GRAY &&
          (int)trans_values->gray > sample_max) ||
          (info_ptr->color_type == PNG_COLOR_TYPE_RGB &&
          ((int)trans_values->red > sample_max ||
          (int)trans_values->green > sample_max ||
          (int)trans_values->blue > sample_max)))
         png_warning(png_ptr,
            "tRNS chunk has out-of-range samples for bit_depth");
      png_memcpy(&(info_ptr->trans_values), trans_values,
         png_sizeof(png_color_16));
      if (num_trans == 0)
         num_trans = 1;
   }

   info_ptr->num_trans = (png_uint_16)num_trans;
   if (num_trans != 0)
   {
      info_ptr->valid |= PNG_INFO_tRNS;
#ifdef PNG_FREE_ME_SUPPORTED
      info_ptr->free_me |= PNG_FREE_TRNS;
#else
      png_ptr->flags |= PNG_FLAG_FREE_TRNS;
#endif
   }
}
