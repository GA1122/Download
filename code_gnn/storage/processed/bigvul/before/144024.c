png_read_init_2(png_structp png_ptr, png_const_charp user_png_ver,
   png_size_t png_struct_size, png_size_t png_info_size)
{
    
   if (png_ptr == NULL)
      return;
#if defined(PNG_STDIO_SUPPORTED) && !defined(_WIN32_WCE)
   if (png_sizeof(png_struct) > png_struct_size ||
      png_sizeof(png_info) > png_info_size)
   {
      char msg[80];
      png_ptr->warning_fn = NULL;
      if (user_png_ver)
      {
        png_snprintf(msg, 80,
           "Application was compiled with png.h from libpng-%.20s",
           user_png_ver);
        png_warning(png_ptr, msg);
      }
      png_snprintf(msg, 80,
         "Application  is  running with png.c from libpng-%.20s",
         png_libpng_ver);
      png_warning(png_ptr, msg);
   }
#endif
   if (png_sizeof(png_struct) > png_struct_size)
   {
      png_ptr->error_fn = NULL;
#ifdef PNG_ERROR_NUMBERS_SUPPORTED
      png_ptr->flags = 0;
#endif
      png_error(png_ptr,
      "The png struct allocated by the application for reading is"
      " too small.");
   }
   if (png_sizeof(png_info) > png_info_size)
   {
      png_ptr->error_fn = NULL;
#ifdef PNG_ERROR_NUMBERS_SUPPORTED
      png_ptr->flags = 0;
#endif
      png_error(png_ptr,
        "The info struct allocated by application for reading is"
        " too small.");
   }
   png_read_init_3(&png_ptr, user_png_ver, png_struct_size);
}
