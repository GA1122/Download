png_read_png(png_structp png_ptr, png_infop info_ptr,
                           int transforms,
                           voidp params)
{
   int row;

   if (png_ptr == NULL)
      return;
#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_INVERT_ALPHA)
       png_set_invert_alpha(png_ptr);
#endif

    
   png_read_info(png_ptr, info_ptr);
   if (info_ptr->height > PNG_UINT_32_MAX/png_sizeof(png_bytep))
      png_error(png_ptr, "Image is too high to process with png_read_png()");

    

#ifdef PNG_READ_16_TO_8_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_STRIP_16)
      png_set_strip_16(png_ptr);
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_STRIP_ALPHA)
      png_set_strip_alpha(png_ptr);
#endif

#if defined(PNG_READ_PACK_SUPPORTED) && !defined(PNG_READ_EXPAND_SUPPORTED)
    
   if (transforms & PNG_TRANSFORM_PACKING)
      png_set_packing(png_ptr);
#endif

#ifdef PNG_READ_PACKSWAP_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_PACKSWAP)
      png_set_packswap(png_ptr);
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_EXPAND)
      if ((png_ptr->bit_depth < 8) ||
          (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE) ||
          (info_ptr->valid & PNG_INFO_tRNS))
         png_set_expand(png_ptr);
#endif

    

#ifdef PNG_READ_INVERT_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_INVERT_MONO)
      png_set_invert_mono(png_ptr);
#endif

#ifdef PNG_READ_SHIFT_SUPPORTED
    
   if ((transforms & PNG_TRANSFORM_SHIFT) && (info_ptr->valid & PNG_INFO_sBIT))
      png_set_shift(png_ptr, &info_ptr->sig_bit);
#endif

#ifdef PNG_READ_BGR_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_BGR)
      png_set_bgr(png_ptr);
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_SWAP_ALPHA)
       png_set_swap_alpha(png_ptr);
#endif

#ifdef PNG_READ_SWAP_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_SWAP_ENDIAN)
      png_set_swap(png_ptr);
#endif

 
#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_INVERT_ALPHA)
       png_set_invert_alpha(png_ptr);
#endif

 
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_GRAY_TO_RGB)
       png_set_gray_to_rgb(png_ptr);
#endif

    

    
   png_read_update_info(png_ptr, info_ptr);

    

#ifdef PNG_FREE_ME_SUPPORTED
   png_free_data(png_ptr, info_ptr, PNG_FREE_ROWS, 0);
#endif
   if (info_ptr->row_pointers == NULL)
   {
      info_ptr->row_pointers = (png_bytepp)png_malloc(png_ptr,
         info_ptr->height * png_sizeof(png_bytep));
      png_memset(info_ptr->row_pointers, 0, info_ptr->height
         * png_sizeof(png_bytep));

#ifdef PNG_FREE_ME_SUPPORTED
      info_ptr->free_me |= PNG_FREE_ROWS;
#endif

      for (row = 0; row < (int)info_ptr->height; row++)
         info_ptr->row_pointers[row] = (png_bytep)png_malloc(png_ptr,
            png_get_rowbytes(png_ptr, info_ptr));
   }

   png_read_image(png_ptr, info_ptr->row_pointers);
   info_ptr->valid |= PNG_INFO_IDAT;

    
   png_read_end(png_ptr, info_ptr);

   PNG_UNUSED(transforms)  
   PNG_UNUSED(params)

}