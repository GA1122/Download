png_write_png(png_structp png_ptr, png_infop info_ptr,
              int transforms, voidp params)
{
   if (png_ptr == NULL || info_ptr == NULL)
      return;

    
   png_write_info(png_ptr, info_ptr);

    

#ifdef PNG_WRITE_INVERT_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_INVERT_MONO)
      png_set_invert_mono(png_ptr);
#endif

#ifdef PNG_WRITE_SHIFT_SUPPORTED
    
   if ((transforms & PNG_TRANSFORM_SHIFT)
               && (info_ptr->valid & PNG_INFO_sBIT))
      png_set_shift(png_ptr, &info_ptr->sig_bit);
#endif

#ifdef PNG_WRITE_PACK_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_PACKING)
       png_set_packing(png_ptr);
#endif

#ifdef PNG_WRITE_SWAP_ALPHA_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_SWAP_ALPHA)
      png_set_swap_alpha(png_ptr);
#endif

#ifdef PNG_WRITE_FILLER_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_STRIP_FILLER_AFTER)
      png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);
   else if (transforms & PNG_TRANSFORM_STRIP_FILLER_BEFORE)
      png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);
#endif

#ifdef PNG_WRITE_BGR_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_BGR)
      png_set_bgr(png_ptr);
#endif

#ifdef PNG_WRITE_SWAP_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_SWAP_ENDIAN)
      png_set_swap(png_ptr);
#endif

#ifdef PNG_WRITE_PACKSWAP_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_PACKSWAP)
      png_set_packswap(png_ptr);
#endif

#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
    
   if (transforms & PNG_TRANSFORM_INVERT_ALPHA)
      png_set_invert_alpha(png_ptr);
#endif

    

    
   if (info_ptr->valid & PNG_INFO_IDAT)
       png_write_image(png_ptr, info_ptr->row_pointers);

    
   png_write_end(png_ptr, info_ptr);

   PNG_UNUSED(transforms)  
   PNG_UNUSED(params)
}
