 update_display(struct display *dp)
   
 {
    png_structp pp;
    png_infop   ip;
 
   
    read_png(dp, &dp->original_file, "original read", 0 );
 
   
    dp->original_pp = pp = dp->read_pp, dp->read_pp = NULL;
    dp->original_ip = ip = dp->read_ip, dp->read_ip = NULL;
 
    dp->original_rowbytes = png_get_rowbytes(pp, ip);
  if (dp->original_rowbytes == 0)
       display_log(dp, LIBPNG_BUG, "png_get_rowbytes returned 0");
 
    dp->chunks = png_get_valid(pp, ip, 0xffffffff);
  if ((dp->chunks & PNG_INFO_IDAT) == 0)  
       display_log(dp, LIBPNG_BUG, "png_read_png did not set IDAT flag");
 
    dp->original_rows = png_get_rows(pp, ip);
  if (dp->original_rows == NULL)
       display_log(dp, LIBPNG_BUG, "png_read_png did not create row buffers");
 
  if (!png_get_IHDR(pp, ip,
  &dp->width, &dp->height, &dp->bit_depth, &dp->color_type,
  &dp->interlace_method, &dp->compression_method, &dp->filter_method))
       display_log(dp, LIBPNG_BUG, "png_get_IHDR failed");
 
   
  {
       png_uint_32 chunks = dp->chunks;
  int active = 0, inactive = 0;
  int ct = dp->color_type;
 
        int bd = dp->bit_depth;
        unsigned int i;
  
      for (i=0; i<TTABLE_SIZE; ++i)
//       for (i=0; i<TTABLE_SIZE; ++i) if (transform_info[i].name != NULL)
        {
           int transform = transform_info[i].transform;
  
           if ((transform_info[i].valid_chunks == 0 ||
                 (transform_info[i].valid_chunks & chunks) != 0) &&
            (transform_info[i].color_mask_required & ct) == 
//             (transform_info[i].color_mask_required & ct) ==
                 transform_info[i].color_mask_required &&
              (transform_info[i].color_mask_absent & ct) == 0 &&
              (transform_info[i].bit_depths & bd) != 0 &&
  (transform_info[i].when & TRANSFORM_R) != 0)
             active |= transform;
 
  else if ((transform_info[i].when & TRANSFORM_R) != 0)
             inactive |= transform;
  }
 
   
       inactive &= ~active;
 
  
        dp->active_transforms = active;
        dp->ignored_transforms = inactive;  
      if (active == 0)
         display_log(dp, INTERNAL_ERROR, "bad transform table");
     }
  }