png_get_compression_buffer_size(png_structp png_ptr)
{
   return (png_uint_32)(png_ptr? png_ptr->zbuf_size : 0L);
}
