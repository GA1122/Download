png_handle_PLTE(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
   png_color palette[PNG_MAX_PALETTE_LENGTH];
   int max_palette_length, num, i;
#ifdef PNG_POINTER_INDEXING_SUPPORTED
   png_colorp pal_ptr;
#endif

   png_debug(1, "in png_handle_PLTE");

   if ((png_ptr->mode & PNG_HAVE_IHDR) == 0)
      png_chunk_error(png_ptr, "missing IHDR");

    
   else if ((png_ptr->mode & PNG_HAVE_PLTE) != 0)
      png_chunk_error(png_ptr, "duplicate");

   else if ((png_ptr->mode & PNG_HAVE_IDAT) != 0)
   {
       
      png_crc_finish(png_ptr, length);
      png_chunk_benign_error(png_ptr, "out of place");
      return;
   }

   png_ptr->mode |= PNG_HAVE_PLTE;

   if ((png_ptr->color_type & PNG_COLOR_MASK_COLOR) == 0)
   {
      png_crc_finish(png_ptr, length);
      png_chunk_benign_error(png_ptr, "ignored in grayscale PNG");
      return;
   }

#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   if (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      png_crc_finish(png_ptr, length);
      return;
   }
#endif

   if (length > 3*PNG_MAX_PALETTE_LENGTH || length % 3)
   {
      png_crc_finish(png_ptr, length);

      if (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
         png_chunk_benign_error(png_ptr, "invalid");

      else
         png_chunk_error(png_ptr, "invalid");

      return;
   }

    
   num = (int)length / 3;

    
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      max_palette_length = (1 << png_ptr->bit_depth);
   else
      max_palette_length = PNG_MAX_PALETTE_LENGTH;

   if (num > max_palette_length)
      num = max_palette_length;

#ifdef PNG_POINTER_INDEXING_SUPPORTED
   for (i = 0, pal_ptr = palette; i < num; i++, pal_ptr++)
   {
      png_byte buf[3];

      png_crc_read(png_ptr, buf, 3);
      pal_ptr->red = buf[0];
      pal_ptr->green = buf[1];
      pal_ptr->blue = buf[2];
   }
#else
   for (i = 0; i < num; i++)
   {
      png_byte buf[3];

      png_crc_read(png_ptr, buf, 3);
       
      palette[i].red = buf[0];
      palette[i].green = buf[1];
      palette[i].blue = buf[2];
   }
#endif

    
#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#endif
   {
      png_crc_finish(png_ptr, (png_uint_32) (length - (unsigned int)num * 3));
   }

#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   else if (png_crc_error(png_ptr) != 0)   
   {
       
      if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_USE) == 0)
      {
         if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN) != 0)
            return;

         else
            png_chunk_error(png_ptr, "CRC error");
      }

       
      else if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN) == 0)
         png_chunk_warning(png_ptr, "CRC error");
   }
#endif

    
   png_set_PLTE(png_ptr, info_ptr, palette, num);

    
#ifdef PNG_READ_tRNS_SUPPORTED
   if (png_ptr->num_trans > 0 ||
       (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS) != 0))
   {
       
      png_ptr->num_trans = 0;

      if (info_ptr != NULL)
         info_ptr->num_trans = 0;

      png_chunk_benign_error(png_ptr, "tRNS must be after");
   }
#endif

#ifdef PNG_READ_hIST_SUPPORTED
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_hIST) != 0)
      png_chunk_benign_error(png_ptr, "hIST must be after");
#endif

#ifdef PNG_READ_bKGD_SUPPORTED
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_bKGD) != 0)
      png_chunk_benign_error(png_ptr, "bKGD must be after");
#endif
}
