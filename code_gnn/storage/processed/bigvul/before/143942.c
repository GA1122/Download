png_reset_zstream(png_structp png_ptr)
{
   if (png_ptr == NULL)
      return Z_STREAM_ERROR;
   return (inflateReset(&png_ptr->zstream));
}
