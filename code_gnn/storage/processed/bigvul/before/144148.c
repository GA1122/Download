png_write_sRGB(png_structp png_ptr, int srgb_intent)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_sRGB;
#endif
   png_byte buf[1];

   png_debug(1, "in png_write_sRGB");

   if (srgb_intent >= PNG_sRGB_INTENT_LAST)
         png_warning(png_ptr,
            "Invalid sRGB rendering intent specified");
   buf[0]=(png_byte)srgb_intent;
   png_write_chunk(png_ptr, (png_bytep)png_sRGB, buf, (png_size_t)1);
}
