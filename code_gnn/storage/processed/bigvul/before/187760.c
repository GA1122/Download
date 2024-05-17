 read_png(struct control *control)
   
 
  {
     png_structp png_ptr;
     png_infop info_ptr = NULL;
   volatile png_bytep row = NULL, display = NULL;
     volatile int rc;
  
     png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, control,
       error_handler, warning_handler);
 
  if (png_ptr == NULL)
  {
   
       log_error(&control->file, LIBPNG_ERROR_CODE, "OOM allocating png_struct");
       control->file.status_code |= INTERNAL_ERROR;
  return LIBPNG_ERROR_CODE;
  }
 
 
     rc = setjmp(control->file.jmpbuf);
     if (rc == 0)
     {
// #     ifdef PNG_SET_USER_LIMITS_SUPPORTED
//           
//          png_set_user_limits(png_ptr, 0x7fffffff, 0x7fffffff);
//          png_set_chunk_cache_max(png_ptr, 0);
//          png_set_chunk_malloc_max(png_ptr, 0);
// #     endif
// 
        png_set_read_fn(png_ptr, control, read_callback);
  
        info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL)
          png_error(png_ptr, "OOM allocating info structure");
 
  if (control->file.global->verbose)
          fprintf(stderr, " INFO\n");
 
 
        png_read_info(png_ptr, info_ptr);
  
        {
         png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
//         png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
//         int passes = png_set_interlace_handling(png_ptr);
//         int pass;
  
         row = png_voidcast(png_byte*, malloc(rowbytes));
         display = png_voidcast(png_byte*, malloc(rowbytes));
//         png_start_read_image(png_ptr);
  
         if (row == NULL || display == NULL)
            png_error(png_ptr, "OOM allocating row buffers");
//         for (pass = 0; pass < passes; ++pass)
//         {
//            png_uint_32 y = height;
  
         {
            png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
            int passes = png_set_interlace_handling(png_ptr);
            int pass;
            png_start_read_image(png_ptr);
            for (pass = 0; pass < passes; ++pass)
            {
               png_uint_32 y = height;
                
               while (y-- > 0)
                  png_read_row(png_ptr, row, display);
            }
         }
//             
//            while (y-- > 0)
//               png_read_row(png_ptr, NULL, NULL);
//         }
        }
  
        if (control->file.global->verbose)
          fprintf(stderr, " END\n");
 
   
       png_read_end(png_ptr, info_ptr);
 
     }
  
     png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   if (row != NULL) free(row);
   if (display != NULL) free(display);
     return rc;
  }