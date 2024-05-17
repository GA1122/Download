png_write_sCAL_s(png_structp png_ptr, int unit, png_charp width,
   png_charp height)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_sCAL;
#endif
   png_byte buf[64];
   png_size_t wlen, hlen, total_len;

   png_debug(1, "in png_write_sCAL_s");

   wlen = png_strlen(width);
   hlen = png_strlen(height);
   total_len = wlen + hlen + 2;
   if (total_len > 64)
   {
      png_warning(png_ptr, "Can't write sCAL (buffer too small)");
      return;
   }

   buf[0] = (png_byte)unit;
   png_memcpy(buf + 1, width, wlen + 1);       
   png_memcpy(buf + wlen + 2, height, hlen);   

   png_debug1(3, "sCAL total length = %u", (unsigned int)total_len);
   png_write_chunk(png_ptr, (png_bytep)png_sCAL, buf, total_len);
}
