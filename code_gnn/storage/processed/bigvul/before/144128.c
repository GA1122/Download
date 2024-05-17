png_write_bKGD(png_structp png_ptr, png_color_16p back, int color_type)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_bKGD;
#endif
   png_byte buf[6];

   png_debug(1, "in png_write_bKGD");

   if (color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (
#ifdef PNG_MNG_FEATURES_SUPPORTED
          (png_ptr->num_palette ||
          (!(png_ptr->mng_features_permitted & PNG_FLAG_MNG_EMPTY_PLTE))) &&
#endif
         back->index >= png_ptr->num_palette)
      {
         png_warning(png_ptr, "Invalid background palette index");
         return;
      }
      buf[0] = back->index;
      png_write_chunk(png_ptr, (png_bytep)png_bKGD, buf, (png_size_t)1);
   }
   else if (color_type & PNG_COLOR_MASK_COLOR)
   {
      png_save_uint_16(buf, back->red);
      png_save_uint_16(buf + 2, back->green);
      png_save_uint_16(buf + 4, back->blue);
      if (png_ptr->bit_depth == 8 && (buf[0] | buf[2] | buf[4]))
      {
         png_warning(png_ptr,
           "Ignoring attempt to write 16-bit bKGD chunk when bit_depth is 8");
         return;
      }
      png_write_chunk(png_ptr, (png_bytep)png_bKGD, buf, (png_size_t)6);
   }
   else
   {
      if (back->gray >= (1 << png_ptr->bit_depth))
      {
         png_warning(png_ptr,
           "Ignoring attempt to write bKGD chunk out-of-range for bit_depth");
         return;
      }
      png_save_uint_16(buf, back->gray);
      png_write_chunk(png_ptr, (png_bytep)png_bKGD, buf, (png_size_t)2);
   }
}
